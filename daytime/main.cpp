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
	const int server_port = 13;
	int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (client_socket == -1) {
		std::cerr << "Ошибка при создании сокета" << std::endl;
		return 1;
	}
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	server_address.sin_addr.s_addr = inet_addr(server_ip);
	const char* message = "";
	ssize_t sent_bytes = sendto(client_socket, message,
		strlen(message), 0, (struct sockaddr*)&server_address,
		sizeof(server_address));
	if (sent_bytes == -1) {
		std::cerr << "Ошибка при отправке сообщения" << std::endl;
		close(client_socket);
		return 1;
	}
	char buffer[4096];
	memset(buffer, 0, sizeof(buffer));
	socklen_t server_address_len = sizeof(server_address);
	ssize_t received_bytes = recvfrom(client_socket, buffer,
		sizeof(buffer), 0, (struct sockaddr*)&server_address,
		&server_address_len);
	if (received_bytes == -1) {
		std::cerr << "Ошибка при получении ответа от сервера" << std::endl;
		close(client_socket);
		return 1;
	}
	std::cout << "Ответ от сервера: " << buffer << std::endl;
	close(client_socket);
	return 0;
}
