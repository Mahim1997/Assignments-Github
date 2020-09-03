package problem2;

public interface Handler {

    public static final int validResponseID = 200;
    public static final int notFoundID = 404;
    public static String defaultInvalid = "NOT APPLICABLE PLEASE CHECK RESPONSE ID!!";
    public static String defaultFileName = "index.html";

    public static String errorPageName = "error_404.html";
    public static String defaultHTMLFormat = ".html";

    public static void replacePercentagesWithSpaces(String[] arr){
        for(int i=0; i<arr.length; i++){
            String s = arr[i];
            s = s.replaceAll("%20", " ");
            arr[i] = s;
        }
    }
    
    public Info handle();

    public static void printArray(String[] arr) {
        for (String s : arr) {
            System.out.println(s);
        }
    }

    public static void trimArray(String[] arr) {
        for (int i = 0; i < arr.length; i++) {
            arr[i] = arr[i].trim();
        }
    }

    public static String getResponse(int id) {
        String finalResponse;
        String responseType;
        String responseNumber;
        int responseID; //same as responseNumber
        if (id == validResponseID) {
            responseType = "OK";

        } else if (id == notFoundID) {
            responseType = "Not Found";
        } else {
            finalResponse = "NOT APPLICABLE PLEASE CHECK RESPONSE ID!!";
//            System.err.print(this.finalResponse);
            return "-1";
        }
        responseNumber = String.valueOf(id);
        finalResponse = responseType + " " + responseNumber;
        return finalResponse;
    }
}
