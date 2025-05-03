package Client;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.Scanner;

public class Client {
    private static final int PORT=5013;
    public static void main(String[] args) throws Exception {

        try {
            while (true) {
                Socket serverSocket = new Socket("localhost", PORT);
                System.out.println("Connection established");
                System.out.println("Remote port: " + serverSocket.getPort());
                System.out.println("Local port: " + serverSocket.getLocalPort());
                Scanner scanner = new Scanner(System.in);
                String input = scanner.nextLine();
                if (input==null) continue;
                System.out.println(input);
                ClientThread clientThread = new ClientThread(serverSocket, input);
                clientThread.start();
            }
        }catch (Exception e){
            e.printStackTrace();
            //serverSocket.close();
        }
    }
}

