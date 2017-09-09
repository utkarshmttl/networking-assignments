#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
using namespace std;

float sum(float a, float b){
    return a+b;
}

int main()
{
    
    int client, server;
    int portNum = 8050;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    int optval = 1;
    pid_t pid;

    struct sockaddr_in server_addr;
    socklen_t size; //sys/socket.h

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    client = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(client,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int));

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }


    cout << "\n=> Socket server has been created..." << endl;

    

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    /* ---------- BINDING THE SOCKET ---------- */
    /* ---------------- bind() ---------------- */


    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }

    /* 
        The bind() system call binds a socket to an address, 
        in this case the address of the current host and port number 
        on which the server will run. It takes three arguments, 
        the socket file descriptor. The second argument is a pointer 
        to a structure of type sockaddr, this must be cast to
        the correct type.
    */

    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;

    /* ------------- LISTENING CALL ------------- */

    listen(client, 5);

    /* 
        The listen system call allows the process to listen 
        on the socket for connections. 
        The program will be stay idle here if there are no 
        incomming connections.
        The first argument is the socket file descriptor, 
        and the second is the size for the number of clients 
        i.e the number of connections that the server can 
        handle while the process is handling a particular 
        connection. The maximum size permitted by most 
        systems is 5.
    */

    /* ------------- ACCEPTING CLIENTS  ------------- */

    /* 
        The accept() system call causes the process to block 
        until a client connects to the server. Thus, it wakes 
        up the process when a connection from a client has been 
        successfully established. It returns a new file descriptor, 
        and all communication on this connection should be done 
        using the new file descriptor. The second argument is a 
        reference pointer to the address of the client on the other 
        end of the connection, and the third argument is the size 
        of this structure.
    */

    int clientCount = 0;
    while(1){
    server = accept(client,(struct sockaddr *)&server_addr,&size);
    clientCount++;

    // first check if it is valid or not
    if (server < 0) 
        cout << "=> Error on accepting..." << endl;

    else 
    {
    	pid = fork();
    	if(pid<0){						
    		cout<<"Unable to create new process for new client..."<<endl;
    	}
    	if(pid==0){
    	close(client);
    	memset(&buffer, '\0', sizeof(buffer));
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
        cout << "\n=> Enter # to end the connection\n" << endl;

        /* 
            we get to this point only after a 
            client has successfully connected to our server. 
            This reads from the socket. Note that the read() 
            will block until there is something for it to read 
            in the socket, i.e. after the client has executed a 
            the send().
            It will read either the total number of characters 
            in the socket or 1024
        */

        // cout << "Client: ";
        // do {
        //     cout<<"faaltu wala loop \n";
        //     recv(server, buffer, bufsize, 0);
        //     cout << buffer << " ";
        //     if (*buffer == '#') {
        //         *buffer = '*';
        //         isExit = true;
        //     }
        // } while (*buffer != '*');

        // do {
        //     cout << "\nServer: ";
        //     do {
        //         cin >> buffer;
        //         send(server, buffer, bufsize, 0);
        //         if (*buffer == '#') {
        //             send(server, buffer, bufsize, 0);
        //             *buffer = '*';
        //             isExit = true;
        //         }
        //     } while (*buffer != '*');

        //     cout << "Client: ";
        //     do {
        //         recv(server, buffer, bufsize, 0);
        //         cout << buffer << " ";
        //         if (*buffer == '#') {
        //             *buffer == '*';
        //             isExit = true;
        //         }
        //     } while (*buffer != '*');
        // } while (!isExit);



        // do {

        //     cout<<"server outermost loop\n";
        //     cout << "Client: ";
        //     do {
        //         cout<<"server receive loop\n";
        //         recv(server, buffer, bufsize, 0);
        //         cout << buffer << " ";
        //         if (*buffer == '#') {
        //             *buffer == '*';
        //             isExit = true;
        //         }
        //     } while (*buffer != '*');

        //     cout << "\nServer: ";
        //     do {
        //         cout<<"server send loop\n";
        //         cin >> buffer;
        //         send(server, buffer, bufsize, 0);
        //         if (*buffer == '#') {
        //             send(server, buffer, bufsize, 0);
        //             *buffer = '*';
        //             isExit = true;
        //         }
        //     } while (*buffer != '*');

            
        // } while (!isExit);
        float a;
        float b;
        float result;

        do {


            
            memset(&buffer, '\0', sizeof(buffer));
            recv(server, buffer, bufsize, 0);
            if(!strcmp(buffer,"sum")){
            	memset(&buffer, '\0', sizeof(buffer));
                strcpy(buffer,"enter first number");
                send(server, buffer, bufsize, 0);
                memset(&buffer, '\0', sizeof(buffer));
                recv(server, buffer, bufsize, 0);
                a = atof(buffer);
                cout<<endl<<a<<endl;

                memset(&buffer, '\0', sizeof(buffer));
                strcpy(buffer,"enter second number");
                send(server, buffer, bufsize, 0);
                memset(&buffer, '\0', sizeof(buffer));
                recv(server, buffer, bufsize, 0);
                b = atof(buffer);

                cout<<endl<<b<<endl;
                result=sum(a,b);
                cout<<endl<<result<<endl;

                send(server, &result, sizeof result, 0);
                //send(server, buffer, bufsize, 0);
            }
            else if(!strcmp(buffer,"cd")){
                memset(&buffer, '\0', sizeof(buffer));
                recv(server, buffer, bufsize, 0);//recvd direc name
                chdir(buffer);

                memset(&buffer, '\0', sizeof(buffer));
                strcpy(buffer,"finished cd");
                send(server, buffer, bufsize, 0);



                



            }
            else{
            cout << "\nClient "<<clientCount<<": ";
            cout << buffer << " ";
            system("touch .commandOutput");
            strcat(buffer," > .commandOutput \n");
            system(buffer);
            if(*buffer == '#') break;

            cout<< "\nServer: ";
            //cin.getline(buffer, bufsize);
             // ifstream ifs("commandOutput");
             // string content( (std::istreambuf_iterator<char>(ifs) ),
             //            (std::istreambuf_iterator<char>()    ) );
             // //cout<<content<<endl;
             // sprintf(buffer, "%.4s", content.c_str());
             // cout<<buffer<<endl;


            int position = 0;
            ifstream fin(".commandOutput"); //opening an input stream for file test.txt
                    /*checking whether file could be opened or not. If file does not exist or don't have read permissions, file
                  stream could not be opened.*/
                  if(fin.is_open())
                    {
                    //file opened successfully so we are here
                    //cout << "File Opened successfully!!!. Reading data from file into array" << endl;
                    //this loop run until end of file (eof) does not occur
                        while(!fin.eof() && position < bufsize)
                        {
                            fin.get(buffer[position]); //reading one character from file to array
                            position++;
                        }
                        buffer[position-1] = '\0'; //placing character array terminating character

                    //cout << "Displaying Array..." << endl << endl;
                    //this loop display all the charaters in array till \0 
                        // for(int i = 0; buffer[i] != '\0'; i++)
                        // {
                        //     cout << buffer[i];
                        // }
                    }
            system("rm .commandOutput");
            cout<<buffer<<endl;
            send(server, buffer, bufsize, 0);
            if(*buffer == '#') break;
            }
                        

        } while (1);

        /* ----------------- close() --------------- */

        /* 
            Once the server presses # to end the connection,
            the loop will break and it will close the server 
            socket connection and the client connection.
        */

        // inet_ntoa converts packet data to IP, which was taken from client
        cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        if(!close(server)){
         //cout<<"server filedes on server closed";
        }
        cout << "\nGoodbye..." << endl;
        isExit = false;
        //exit(1);
        break;

    	}
    	else{
    		close(server);
            // pid_t result = waitpid(pid,NULL,WNOHANG);
            // if(!(result==0 || result==-1)){clientCount--;}
    	}
    }

	}

    if(!close(client)) {
    //cout<<"client filedes on server closed";
    }
    clientCount--;
    return 0;
}


//fixed problem of error binding using netstat command and reuseaddr setsocketopt