#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include <vector>
#include <sys/dir.h>
#include <sys/param.h>
#include <fstream>

using namespace std;

string ls()
{
    int count,i;
    struct dirent **files;
    int file_select(const struct dirent*);
    char pathname[MAXPATHLEN];
    if (getcwd(pathname, sizeof(pathname)) == NULL)
    {
        cout<<"Error getting path"<<endl;
    }
    count = scandir(pathname, &files, file_select, alphasort);
    vector<string> filename(count);
    string result;
    if (count <= 0)
    {
        cout<<"No files in this directory"<<endl;
    }
    else
    {
        for (i=1;i<count+1;++i)
        {
            //cout<<files[i-1]->d_name<<" ";
            filename.at(i-1) = files[i-1]->d_name;
        }
    }
    for (auto const& s : filename) { result += s; result += " "; }
    return result;
}

int file_select(const struct dirent *entry)
{
    if ((entry->d_name == ".") ||(entry->d_name == "..")){
        return (0);
    }
    
    else
    return (1);
}

int main()
{
    
    int client, server;
    int portNum = 8080;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    int optval = 1;
    int pid;

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
    cout << "=> Looking for clients..." << endl;

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
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        strcpy(buffer,"\n");
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

        string lsresult;
        int filesize;
        char buffer_first_word[bufsize];
        char buffer_second_word[bufsize]; //filename
        int first_space_index;
        int first_word_end_index;
        int second_word_end_index;

        do {
            strcpy(buffer,"\n");
            cout<<"loop start buffer: "<<buffer<<endl;

            
            recv(server, buffer, bufsize, 0);
            for(int iter=0;iter < bufsize;iter++){
                if(buffer[iter] == ' ' ){ first_space_index=iter; first_word_end_index=iter; break;}
                buffer_first_word[iter] = buffer[iter];
            }
            buffer_first_word[first_word_end_index] = 0;
            cout<<"first word: "<<buffer_first_word<<endl;
            for(int iter=first_space_index+1;iter < bufsize;iter++){
                if(buffer[iter] == ' '){second_word_end_index=iter-(first_space_index+1); break;}
                buffer_second_word[iter-(first_space_index+1)] = buffer[iter];
            }
            buffer_second_word[second_word_end_index] = 0;
            //cout<<buffer_first_word;
            cout<<"second word: "<<buffer_second_word<<endl;
            //cout<<"hereeeeee: "<<buffer_second_word<<endl;;
            cout << "\nClient "<<clientCount<<": ";
            cout << buffer << " ";
            //if(buffer == 'ls'){
            if(!strcmp(buffer,"serverls")){
                
                lsresult = ls();
                    if(*buffer == '#') break;

                    cout<< "\nServer: ";
                    //cin.getline(buffer, bufsize);
                    cout<<lsresult;
                    cout<<endl;
                    strcpy(buffer, lsresult.c_str());
                    send(server, buffer, bufsize, 0);
                    if(*buffer == '#') break;
            }
            else if(!strcmp(buffer_first_word,"upload")){
                cout<<endl<<"upload match on server"<<endl;
                //cout<<"upload match worked"<<endl;
                recv(server, buffer, bufsize, 0);
                strcpy(buffer_second_word,buffer); //filename
                //filesize = atoi(buffer);
                //cout<<filesize<<endl;
                cout<<"filename:  "<<buffer_second_word<<endl;

                recv(server, buffer, bufsize, 0);
                filesize = atoi(buffer);
                cout<<endl<<"filesize: "<<filesize<<endl;
            }
            else if(!strcmp(buffer_first_word,"download")){
                //cout<<"download match worked"<<endl;
            }
            else{
            if(*buffer == '#') break;

            cout<< "\nServer: ";
            cin.getline(buffer, bufsize);
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