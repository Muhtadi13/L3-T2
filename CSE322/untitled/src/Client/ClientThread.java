package Client;

import java.io.*;
import java.net.Socket;
import java.util.Arrays;

public class ClientThread extends Thread{
    private final Socket serverSocket;
    private final String fileName;
    BufferedReader bufferedReader;
    BufferedWriter bufferedWriter;
    DataInputStream dataInputStream;
    DataOutputStream dataOutputStream;
    private final  String rootDirectory;
    private final int CHUNK = 4096;
    public ClientThread(Socket serverSocket,String fileName) throws Exception{
        this.serverSocket=serverSocket;
        this.fileName = fileName;
        bufferedReader=new BufferedReader(new InputStreamReader(this.serverSocket.getInputStream()));
        bufferedWriter=new BufferedWriter(new OutputStreamWriter(this.serverSocket.getOutputStream()));
        dataInputStream=new DataInputStream(this.serverSocket.getInputStream());
        dataOutputStream=new DataOutputStream(this.serverSocket.getOutputStream());
        rootDirectory = "C:/Users/almuh/Desktop";

    }
    @Override
    public void run() {
        try {
            String path = rootDirectory.concat("/").concat(fileName);
            File file = new File(path);
            System.out.println(path);
            if (!file.exists()) {
                throw new FileNotFoundException();
            }
            System.out.println(path);
            FileInputStream fin=new FileInputStream(file);
            if(canStartUploading(file)){
                readWriteInChunk(fin,dataOutputStream);
            }else{
                System.out.println("Invalid File Format");
            }
            bufferedWriter.close();
            bufferedReader.close();
            dataOutputStream.close();
            dataInputStream.close();
            fin.close();
        }catch (Exception e){
            if(e instanceof FileNotFoundException){
                System.out.println("File Not Found");
            }else {
                e.printStackTrace();
            }
        }
    }
    private void readWriteInChunk(InputStream in,OutputStream out) throws IOException {
        byte[] buffer = new byte[CHUNK];
        int i;
        while ((i = in.read(buffer)) != -1) {
            //System.out.println(Arrays.toString(buffer));
            out.write(buffer,0,i);
            out.flush();
        }
    }
    private boolean canStartUploading(File file) throws IOException {
        bufferedWriter.write("uploading ".concat(fileName)+"\r\n");
        bufferedWriter.flush();
        System.out.println("message sent from "+file.getAbsolutePath()+" to "+serverSocket.getPort());
        String confirmation=bufferedReader.readLine();
        System.out.println(confirmation);
        return confirmation.equalsIgnoreCase("start");
    }
}
