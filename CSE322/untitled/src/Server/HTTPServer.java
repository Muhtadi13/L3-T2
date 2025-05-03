package Server;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
public class HTTPServer {
    static final int PORT = 5013;

    public static void main(String[] args) throws IOException {
        try {
            ServerSocket serverConnect = new ServerSocket(PORT);
            System.out.println("Server started.\nListening for connections on port : " + PORT + " ...\n");
            while (true) {
                System.out.println("Waiting for connection...");
                Socket clientSocket = serverConnect.accept();
                System.out.println("Connection established to "+clientSocket.getPort());
                Thread worker = new ServerThread(clientSocket);
                worker.start();
            }
        }catch (Exception e){
            //e.printStackTrace();
            System.out.println("Client Disconnected");
        }
        
    }
    
}
