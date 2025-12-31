package Server;

import netscape.javascript.JSObject;

import java.io.*;
import java.net.Socket;
import java.net.URLConnection;
import java.util.*;
import java.nio.file.Files;

public class ServerThread extends Thread {
    Socket socket;
    DataInputStream datainputstream;
    DataOutputStream dataOutputStream;
    BufferedReader bufferedReader;
    BufferedWriter bufferedWriter;
    PrintWriter printWriter;
    private final int CHUNK = 4096;
    private final String rootDirectory;
    private final String logFileDirectory;
    FileOutputStream logOutputStream;

    public ServerThread(Socket socket) throws IOException {
        this.socket = socket;
        datainputstream = new DataInputStream(this.socket.getInputStream());
        bufferedReader=new BufferedReader(new InputStreamReader(this.socket.getInputStream()));
        bufferedWriter=new BufferedWriter(new OutputStreamWriter(this.socket.getOutputStream()));
        dataOutputStream=new DataOutputStream(this.socket.getOutputStream());
        printWriter = new PrintWriter(this.socket.getOutputStream());
        rootDirectory="C:/Users/almuh/Desktop/root";
        logFileDirectory=rootDirectory+"/log.txt";
        logOutputStream=new FileOutputStream(logFileDirectory,true);

    }

    public static String readFileData(File file, long fileLength) throws IOException {
        FileInputStream fileIn = null;
        byte[] fileData = new byte[(int) fileLength];
        String encodedString;
        try {
            fileIn = new FileInputStream(file);
            fileIn.read(fileData);
            encodedString = Base64.getEncoder().encodeToString(fileData);
        } finally {
            if (fileIn != null)
                fileIn.close();
        }
        return encodedString;
    }

    private boolean validUploadRequest(String fileName){
        String fileType = URLConnection.guessContentTypeFromName(fileName);
        System.out.println(fileType);
        return (fileType!=null && (fileType.startsWith("image") || fileType.startsWith("text")));
    }

    private void writeIntoLogFile(String request,String response) throws IOException {
        String modReq="Request : "+request+"\r\n";
        String modRes="Response : "+response+"\r\n\r\n";
        logOutputStream.write((modReq+modRes).getBytes());
        logOutputStream.flush();
    }

    public void run(){
        try {
            String input = bufferedReader.readLine();
            System.out.println("input : "+input);

            // String content = "<html>Hello</html>";
           // input.startsWith("s")
            if(input!=null && input.length() > 0) {
                if(input.startsWith("GET")) {
                    //String segments[]=input.split("[/ ]");
                    //for(String el : segments){
                        //   System.out.println(el);
                    //}
                    //}
                    String fileName = getFilePath(input);
                    String path = rootDirectory.concat(fileName);
                    File file = new File(path);
                    System.out.println(fileName);
                    try {
                        if (!file.exists()) {
                            showError(input);
                        }else if(file.isDirectory()){
                            System.out.println("directory detected");
                            showListInDirectory(input,file,fileName);
                        }else{
                            System.out.println("file detected");
                            getFile(input,file);
                        }
                    }catch (Exception e){
                        e.printStackTrace();
                    }
                } else if(input.startsWith("uploading")) {
                    String[] segments =input.split(" ");
                    System.out.println(segments[1]);
                    if(validUploadRequest(segments[1])) {
                        System.out.println("Valid request");
                        handleUploadFileRequest(segments[1]);
                    }else{
                        //System.out.println("invalid req");
                        handleInvalidRequest();
                    }
                }
            }else{
                System.out.println("Nothing in input");
            }
        } catch (Exception e) {
            //e.printStackTrace();
            try {
                closeConnection();
            } catch (Exception ex) {
                throw new RuntimeException(ex);
            }
        }finally {
            try {
                closeConnection();
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }
    }
    public void closeConnection() throws Exception{
        socket.close();
        logOutputStream.close();
    }
    public void showError(String request) throws IOException {
        StringBuilder response = new StringBuilder();
        response.append("HTTP/1.0 404 Not Found\r\n")
                .append("Content-Type: text/html\r\n")
                .append("\r\n")
                .append("<HTML><body> 404 Page Not Found</body></HTML>");

        printWriter.write(response.toString());
        System.out.println(response);
        printWriter.flush();
        printWriter.close();
        writeIntoLogFile(request,response.toString());
    }
    private void showListInDirectory(String request,File file, String path) throws IOException {
        StringBuilder response = new StringBuilder();

        response.append("HTTP/1.0 200 OK\r\n")
                .append("Content-Type: text/html\r\n")
                .append("\r\n");

        response.append("<html><body>")
                .append("<h1>Directory list for ").append(path).append("</h1>")
                .append("<ul>");

        String newPath;
        if(path.endsWith("/")){
            newPath=path;
        }else{
            newPath=path+"/";
        }

        if(file.listFiles() != null) {
            for (File oneFile : Objects.requireNonNull(file.listFiles())) {
                if (oneFile.isDirectory()) {
                    response.append("<li><a href=\"").append(newPath).append(oneFile.getName()).append("/\"><b><i>")
                            .append(oneFile.getName()).append("</i></b></a></li>");
                } else {
                    response.append("<li><a href=\"").append(newPath).append(oneFile.getName()).append("\">")
                            .append(oneFile.getName()).append("</a></li>");
                }
                System.out.println(response);
            }
        }
        response.append("</ul>")
                .append("</body></html>");
        System.out.println(response);
        printWriter.write(response.toString());
        printWriter.flush();
        printWriter.close();
        writeIntoLogFile(request,response.toString());
    }
    private String getFilePath(String request){
        String[] segments =request.split(" ");
        return segments[1];
    }
    private void getFile(String request,File file) throws Exception {
        String mimeType=Files.probeContentType(file.toPath());
        boolean isTextOrImage = mimeType != null && (mimeType.startsWith("text") || mimeType.startsWith("image"));
        System.out.println(file.getName());
        StringBuilder response = new StringBuilder();
        if (isTextOrImage) {
            response.append("HTTP/1.0 200 OK\r\n")
                    .append("Server: Java HTTP Server: 1.0\r\n")
                    .append("Date: ").append(new Date()).append("\r\n")
                    .append("Content-Type: ").append(mimeType).append("\r\n")
                    .append("Content-Disposition: inline\r\n")
                    .append("Content-Length: ") .append(file.length()) .append("\r\n")
                    .append("\r\n");
        } else {
            response.append("HTTP/1.0 200 OK\r\n")
                    .append("Content-Disposition: attachment; filename=\"").append(file.getName()).append("\"\r\n")
                    .append("Content-Type: application/octet-stream\r\n")
                    .append("\r\n");
        }
        printWriter.write(response.toString());
        printWriter.flush();
        FileInputStream fis = new FileInputStream(file);
        String content=readWriteInChunk(fis,dataOutputStream);
        fis.close();
        dataOutputStream.close();
        writeIntoLogFile(request,response.toString());
    }

    private String readWriteInChunk(InputStream in,OutputStream out) throws IOException {
        StringBuilder content=new StringBuilder();
        byte[] buffer = new byte[CHUNK];
        int i;
        while ((i = in.read(buffer)) != -1) {
            System.out.println(Arrays.toString(buffer));
            content.append(Arrays.toString(buffer));
            out.write(buffer,0,i);
            out.flush();
        }
        return content.toString();
    }

    private void makeHTMLPage(String request,File file) throws IOException {
        StringBuilder response = new StringBuilder();
        String fileType=Files.probeContentType(file.toPath());
        String content;
        long fileLength=file.length();
        response.append("<html><body>");
        if(fileType.startsWith("text")){
            StringBuilder stringBuilder = new StringBuilder();
            String line;
            while(( line = bufferedReader.readLine()) != null ) {
                System.out.println(line);
                stringBuilder.append( line );
                stringBuilder.append( '\n' );
            }
            content = stringBuilder.toString();
            response.append(content);
        }else {
            System.out.println(file.getName() + " image er nam");
            content = readFileData(file,fileLength);
            response.append("Content-Disposition: inline\r\n")
                    .append("Content-Length: ").append(file.length()).append("\r\n")
                    .append("\r\n");
            response.append(content);
        }
        response.append("</body></html>");
    }
    private void handleUploadFileRequest(String fileName) throws Exception{
        bufferedWriter.write("start\r\n");
        bufferedWriter.flush();
        String uploadPath = rootDirectory.concat("/uploaded/"+fileName);
        FileOutputStream fileOutputStream = new FileOutputStream(uploadPath);
        readWriteInChunk(datainputstream,fileOutputStream);
        datainputstream.close();
        fileOutputStream.close();
        System.out.println("file created at "+uploadPath);
    }
    private void handleInvalidRequest() throws Exception{
        bufferedWriter.write("invalid\r\n");
        bufferedWriter.flush();
        System.out.println("Invalid File");
    }
}
