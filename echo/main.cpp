#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
int main()
{
	const char* server_ip = "172.16.40.1";
	const int server_port = 7;
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		std::cerr << "ОШибка при создании сокета" << std::endl;
		return 1;
	}
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	server_address.sin_addr.s_addr = inet_addr(server_ip);
	int rc = connect(client_socket, (struct
		sockaddr*)&server_address, sizeof(server_address));
	if (rc == -1) {
		std::cerr << "	Ошибка при подключении к серверу" <<
			std::endl;
		close(client_socket);
		return 1;
	}
	const char* message = "Hello, Echo!";
	ssize_t sent_bytes = send(client_socket, message,
		strlen(message), 0);
	if (sent_bytes == -1) {
		std::cerr << "Ошибка при отправке сообщения" << std::endl;
		close(client_socket);
		return 1;
	}
	char buffer[4096];
	memset(buffer, 0 , sizeof(buffer));
	ssize_t received_bytes = recv(client_socket, buffer, sizeof(buffer), 0);
	if (received_bytes == -1) {
		std::cerr << "Ошибка при получении ответа" << std::endl;
		close(client_socket);
		return 1;
	}
	std::cout << "Ответ от сервера: " << buffer << std::endl;
	close(client_socket);
	return 0;
}
