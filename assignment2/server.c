// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    //if no addition args
    if (argc == 1)
    {
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        struct passwd *pwd;
        pid_t pid;

        printf("execve=0x%p\n", execve);

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Attaching socket to port 80
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 80
        if (bind(server_fd, (struct sockaddr *)&address,
                 sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        switch (pid = fork())
        {
        case -1:
            perror("fork failed");
            exit(EXIT_FAILURE);
        case 0:
            //drop privilege
            if (getpwnam("nobody"))
            {
                pwd = getpwnam("nobody");
                setuid(pwd->pw_uid);
                printf("setuid succesful\n");
            }
            else
            {
                perror("nobody user doesn't exist");
                exit(EXIT_FAILURE);
            }
            printf("Child UID after set= %d\n", getuid());
            char formatted_socket[12];
            sprintf(formatted_socket,"%d",new_socket);
            char *args[] = {argv[0], formatted_socket, NULL};
            //re exec child
            int exec = execvp(args[0], args);
            //printf("exec: %d\n",exec);
            if (exec < 0){
                perror("exec failed");
                exit(EXIT_FAILURE);
            }else { printf("exec success");}
            break;
        default:
            close(new_socket);
            wait(NULL);
            printf("Returning child\n");
            break;
        }
        close(server_fd);
    }
    //additional args -> re exec child
    else if (argc > 1)
    {
        int valread;
        char buffer[102] = {0};
        char *hello = "Hello from server";
        new_socket = atoi(argv[1]);

        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message sent from re exec child\n");
    }

    return 0;
}
