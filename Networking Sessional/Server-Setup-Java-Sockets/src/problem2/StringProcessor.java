package problem2;

class StringProcessor {

    private String inputString;
    private String firstLine;
    private Handler handler;

    private String ipAddress;

    public StringProcessor() {
    }

    public void setInput(String s) {
        this.inputString = s;
    }

    public void setIpAddress(String ip){
        this.ipAddress = ip;
    }
    
    Info getResponse() {
        Info info = new Info();

        if ((inputString.length() == 0) || (inputString == null)) {
            return null;
        }

        String[] arr = inputString.split("\n");
        this.firstLine = arr[0];

        if (this.firstLine.contains("GET")) {
            info = new Get_Handler(inputString).handle();
            LogGenerator.writeLog(new Log(info, this.ipAddress));
        } else if (this.firstLine.contains("POST")) {
            info = new Post_Handler(inputString).handle();
            LogGenerator.writeLog(new Log(info, this.ipAddress));
        }

        return info;
    }
}
