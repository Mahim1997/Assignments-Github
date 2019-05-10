package substitution;


public class Mapping {
    char plainTextChar;
    char cipherTextChar;

    public Mapping(char plainTextChar, char cipherTextChar) {
        this.plainTextChar = plainTextChar;
        this.cipherTextChar = cipherTextChar;
    }

    public char getPlainTextChar() {
        return plainTextChar;
    }

    public void setPlainTextChar(char plainTextChar) {
        this.plainTextChar = plainTextChar;
    }

    public char getCipherTextChar() {
        return cipherTextChar;
    }

    public void setCipherTextChar(char cipherTextChar) {
        this.cipherTextChar = cipherTextChar;
    }
    
    
    
}
