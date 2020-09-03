package problem2;

import java.util.Date;

public class Log {

    private String IPAddress;
    private String date;
    private String request;
    private String status;
    private String size;
    private String country;
    private String referer;

    public Log(Info info, String ip){
        this.IPAddress = ip;
        this.date = new Date().toString();
        this.request = info.request;
        this.status = info.status;
        this.size  = String.valueOf(info.file.length());
    }
    
    public Log(String IPAddress, String date, String request, String status, String size) {
        this.IPAddress = IPAddress;
        this.date = date;
        this.request = request;
        this.status = status;
        this.size = size;
    }

    @Override
    public String toString() {
        return  "IPAddress = " + IPAddress + ", date = " + date + ", request = " + request + ", status = " + status + ", size = " + size + "\n";
    }


    public String getIPAddress() {
        return IPAddress;
    }

    public void setIPAddress(String IPAddress) {
        this.IPAddress = IPAddress;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getRequest() {
        return request;
    }

    public void setRequest(String request) {
        this.request = request;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getSize() {
        return size;
    }

    public void setSize(String size) {
        this.size = size;
    }

    public String getCountry() {
        return country;
    }

    public void setCountry(String country) {
        this.country = country;
    }

    public String getReferer() {
        return referer;
    }

    public void setReferer(String referer) {
        this.referer = referer;
    }
    
    
    
}
