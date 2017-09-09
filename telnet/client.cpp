#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()
{
    int client; //file descriptor
    int portNum = 8050;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";
    int optval = 1;

    struct sockaddr_in server_addr; // <netinet/in.h>
    /*
    struct sockaddr_in
        {
          short   sin_family;
          u_short sin_port;
          struct  in_addr sin_addr;
          char    sin_zero[8];
        };
    */

    client = socket(AF_INET, SOCK_STREAM, 0);  //addr domain, type of socket- TCP here, protocol-default 0
    //returns int, -1 on error
    setsockopt(client,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int));

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    cout << "\n=> Socket client has been created..." << endl;
   

    server_addr.sin_family = AF_INET; // sin_family contains a code for the address family. It should always be set to AF_INET.
    server_addr.sin_port = htons(portNum); // htons() converts the port number from host byte order to a port number in network byte order.

    /* ---------- CONNECTING THE SOCKET ---------- */

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << portNum << endl;

    /* 
        The connect function is called by the client to 
        establish a connection to the server. It takes 
        three arguments, the socket file descriptor, the 
        address of the host to which it wants to connect 
        (including the port number), and the size of this 
        address. 
        This function returns 0 on success and -1 
        if it fails.
        Note that the client needs to know the port number of
        the server but not its own port number.
    */

    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    memset(&buffer, '\0', sizeof(buffer));
    recv(client, buffer, bufsize, 0);
    cout<< buffer << " ";
    cout << "=> Connection confirmed, you are good to go..."<<endl;
    cout << "\n=> Enter # to end the connection\n" << endl;

    

    // Once it reaches here, the client can send a message first.

    // do {
    // 	cout<<"client outermost loop\n";
    //     cout << "Client: ";
    //     do {
    //     	cout<<"client send loop\n";
    //         cin >> buffer;
    //         send(client, buffer, bufsize, 0);
    //         if (*buffer == '#') {
    //             send(client, buffer, bufsize, 0);
    //             *buffer = '*';
    //             isExit = true;
    //         }
    //     } while (*buffer != 42);

    //     cout << "Server: ";
    //     do {
    //     	cout<<"client receive loop\n";
    //         recv(client, buffer, bufsize, 0);
    //         cout << buffer << " ";
    //         if (*buffer == '#') {
    //             *buffer = '*';
    //             isExit = true;
    //         }

    //     } while (*buffer != 42);
    //     cout << endl;

    // } while (!isExit);

    float result;

    do {
    	//cout<< "\nClient: ";
        cout<<"\nEnter command to execute on server: ";
    	cin.getline(buffer, bufsize);
        if(!strcmp(buffer,"sum")){
            send(client, buffer, bufsize, 0);

            memset(&buffer, '\0', sizeof(buffer));
            recv(client, buffer, bufsize, 0);
            cout << "\nServer: ";
            cout << buffer << " ";

            cin.getline(buffer, bufsize);
            send(client, buffer, bufsize, 0);

            memset(&buffer, '\0', sizeof(buffer));
            recv(client, buffer, bufsize, 0);
            cout << "\nServer: ";
            cout << buffer << " ";

            cin.getline(buffer, bufsize);
            send(client, buffer, bufsize, 0);


            //recv(client, buffer, bufsize, 0);
            memset(&buffer, '\0', sizeof(buffer));
            recv(client, &result, sizeof result, 0);
            cout << "\nServer: ";
            cout << result << " ";



        }
        else if(!strcmp(buffer,"cd")){
        	send(client, buffer, bufsize, 0);//sending command

        	cout<<"\nEnter directory: ";
        	cin.getline(buffer, bufsize);
        	send(client, buffer, bufsize, 0);//sending direc name

        	memset(&buffer, '\0', sizeof(buffer));
        	recv(client, buffer, bufsize, 0);
            cout << "\nServer: ";
            cout << buffer << " ";

        }
        else{
    	send(client, buffer, bufsize, 0);
    	if(*buffer == '#') break;

    	
    	memset(&buffer, '\0', sizeof(buffer));
    	recv(client, buffer, bufsize, 0);
    	cout << "\nServer: ";
    	cout << buffer << " ";
    	if(*buffer == '#') break;
        }



    } while (1);
    

    


    //----------------------------------------

    /* ---------------- CLOSE CALL ------------- */
    /* ----------------- close() --------------- */

    /* 
        Once the server presses # to end the connection,
        the loop will break and it will close the server 
        socket connection and the client connection.
    */

    cout << "\n=> Connection terminated.\nGoodbye...\n";

    if(!close(client)){
     //cout<<"client filedes on client closed";
    }
    return 0;
}