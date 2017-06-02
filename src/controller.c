#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include "plugin.h"
#include "controller.h"

#include "json.h"
#include "json_tokener.h"

#define HOST "localhost"
#define PORT 8082

int socket_connect(char *host, int portno) {
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd;

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) DebugMessage(M64MSG_ERROR, "ERROR opening socket");

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) DebugMessage(M64MSG_ERROR, "ERROR, no such host");

    /* fill in the structure */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        DebugMessage(M64MSG_INFO, "ERROR connecting, please start bot server.");
        return -1;
    }

    return sockfd;
}

void clear_controller(int control) {
    controller[control].buttons.R_DPAD = 0;
    controller[control].buttons.L_DPAD = 0;
    controller[control].buttons.D_DPAD = 0;
    controller[control].buttons.U_DPAD = 0;
    controller[control].buttons.START_BUTTON = 0;
    controller[control].buttons.Z_TRIG = 0;
    controller[control].buttons.B_BUTTON = 0;
    controller[control].buttons.A_BUTTON = 0;
    controller[control].buttons.R_CBUTTON = 0;
    controller[control].buttons.L_CBUTTON = 0;
    controller[control].buttons.D_CBUTTON = 0;
    controller[control].buttons.U_CBUTTON = 0;
    controller[control].buttons.R_TRIG = 0;
    controller[control].buttons.L_TRIG = 0;
    controller[control].buttons.X_AXIS = 0;
    controller[control].buttons.Y_AXIS = 0;
}

void read_controller() {
    int sockfd = socket_connect(HOST, PORT);

    if (sockfd == -1) {
        clear_controller(0);
        clear_controller(1);
        return;
    }

    int bytes, sent, received, total;
    char message[1024], response[4096]; // allocate more space than required.
    sprintf(message, "GET / HTTP/1.0\r\n\r\n");

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0)
            DebugMessage(M64MSG_ERROR, "ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent += bytes;
    } while (sent < total);

    /* receive the response */
    memset(response, 0, sizeof(response));
    total = sizeof(response) - 1;
    received = 0;
    do {
        bytes = read(sockfd, response + received, total - received);
        if (bytes < 0)
            DebugMessage(M64MSG_ERROR, "ERROR reading response from socket");
        if (bytes == 0)
            break;
        received += bytes;
    } while (received < total);

    if (received == total)
        DebugMessage(M64MSG_ERROR, "ERROR storing complete response from socket");

/* print the response */
#ifdef _DEBUG
    DebugMessage(M64MSG_INFO, response);
#endif

    /* parse the http response */
    char *body = strtok(response, "\n");
    for (int i = 0; i < 5; i++)
        body = strtok(NULL, "\n");

    /* parse the body of the response */
    json_object *jsonObj = json_tokener_parse(body);
    json_object *jsonContr0 = json_object_object_get(jsonObj, "Controller0")
    json_object *jsonContr1 = json_object_object_get(jsonObj, "Controller1")
    
/* print the object */
#ifdef _DEBUG
    DebugMessage(M64MSG_INFO, json_object_to_json_string(jsonObj));
#endif

    controller[0].buttons.R_DPAD = 
        json_object_get_int(json_object_object_get(jsonContr0, "R_DPAD"));
    controller[0].buttons.L_DPAD = 
        json_object_get_int(json_object_object_get(jsonContr0, "L_DPAD"));
    controller[0].buttons.D_DPAD = 
        json_object_get_int(json_object_object_get(jsonContr0, "D_DPAD"));
    controller[0].buttons.U_DPAD = 
        json_object_get_int(json_object_object_get(jsonContr0, "U_DPAD"));
    controller[0].buttons.START_BUTTON = 
        json_object_get_int(json_object_object_get(jsonContr0, "START_BUTTON"));
    controller[0].buttons.Z_TRIG = 
        json_object_get_int(json_object_object_get(jsonContr0, "Z_TRIG"));
    controller[0].buttons.B_BUTTON = 
        json_object_get_int(json_object_object_get(jsonContr0, "B_BUTTON"));
    controller[0].buttons.A_BUTTON = 
        json_object_get_int(json_object_object_get(jsonContr0, "A_BUTTON"));
    controller[0].buttons.R_CBUTTON = 
        json_object_get_int(json_object_object_get(jsonContr0, "R_CBUTTON"));
    controller[0].buttons.L_CBUTTON = 
        json_object_get_int(json_object_object_get(jsonContr0, "L_CBUTTON"));
    controller[0].buttons.D_CBUTTON = 
        json_object_get_int(json_object_object_get(jsonContr0, "D_CBUTTON"));
    controller[0].buttons.U_CBUTTON = 
        json_object_get_int(json_object_object_get(jsonContr0, "U_CBUTTON"));
    controller[0].buttons.R_TRIG = 
        json_object_get_int(json_object_object_get(jsonContr0, "R_TRIG"));
    controller[0].buttons.L_TRIG = 
        json_object_get_int(json_object_object_get(jsonContr0, "L_TRIG"));
    controller[0].buttons.X_AXIS = 
        json_object_get_int(json_object_object_get(jsonContr0, "X_AXIS"));
    controller[0].buttons.Y_AXIS = 
        json_object_get_int(json_object_object_get(jsonContr0, "Y_AXIS"));
        
    controller[1].buttons.R_DPAD = 
        json_object_get_int(json_object_object_get(jsonContr1, "R_DPAD"));
    controller[1].buttons.L_DPAD = 
        json_object_get_int(json_object_object_get(jsonContr1, "L_DPAD"));
    controller[1].buttons.D_DPAD = 
        json_object_get_int(json_object_object_get(jsonContr1, "D_DPAD"));
    controller[1].buttons.U_DPAD = 
        json_object_get_int(json_object_object_get(jsonContr1, "U_DPAD"));
    controller[1].buttons.START_BUTTON = 
        json_object_get_int(json_object_object_get(jsonContr1, "START_BUTTON"));
    controller[1].buttons.Z_TRIG = 
        json_object_get_int(json_object_object_get(jsonContr1, "Z_TRIG"));
    controller[1].buttons.B_BUTTON = 
        json_object_get_int(json_object_object_get(jsonContr1, "B_BUTTON"));
    controller[1].buttons.A_BUTTON = 
        json_object_get_int(json_object_object_get(jsonContr1, "A_BUTTON"));
    controller[1].buttons.R_CBUTTON = 
        json_object_get_int(json_object_object_get(jsonContr1, "R_CBUTTON"));
    controller[1].buttons.L_CBUTTON = 
        json_object_get_int(json_object_object_get(jsonContr1, "L_CBUTTON"));
    controller[1].buttons.D_CBUTTON = 
        json_object_get_int(json_object_object_get(jsonContr1, "D_CBUTTON"));
    controller[1].buttons.U_CBUTTON = 
        json_object_get_int(json_object_object_get(jsonContr1, "U_CBUTTON"));
    controller[1].buttons.R_TRIG = 
        json_object_get_int(json_object_object_get(jsonContr1, "R_TRIG"));
    controller[1].buttons.L_TRIG = 
        json_object_get_int(json_object_object_get(jsonContr1, "L_TRIG"));
    controller[1].buttons.X_AXIS = 
        json_object_get_int(json_object_object_get(jsonContr1, "X_AXIS"));
    controller[1].buttons.Y_AXIS = 
        json_object_get_int(json_object_object_get(jsonContr1, "Y_AXIS"));

    close(sockfd);
}
