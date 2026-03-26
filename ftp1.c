// ftp_client_read.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define SERVER_IP "127.0.0.1" // change if server is remote
#define PORT 5001
#define BUFFER_SIZE 4096

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char filename[256];
    FILE *fp;
    long filesize;
    printf("Enter filename to send: ");
    scanf("%s", filename);
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        perror("File not found");
        exit(1);
    }
    // Get file size
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket error");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connect error");
        exit(1);
    }
    // Send filename
    send(sock, filename, sizeof(filename), 0);
    // Send filesize
    send(sock, &filesize, sizeof(filesize), 0);
    // Send file content
    size_t n;
    while ((n = fread(buffer, 1, BUFFER_SIZE, fp)) > 0)
    {
        send(sock, buffer, n, 0);
    }
    printf("File '%s' sent successfully!\n", filename);
    fclose(fp);
    close(sock);
    return 0;
}
