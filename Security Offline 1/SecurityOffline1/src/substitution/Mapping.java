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

    @Override
    public int hashCode() {
        int hash = 3;
        hash = 67 * hash + this.plainTextChar;
        hash = 67 * hash + this.cipherTextChar;
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Mapping other = (Mapping) obj;
        if (this.plainTextChar != other.plainTextChar) {
            return false;
        }
        if (this.cipherTextChar != other.cipherTextChar) {
            return false;
        }
        return true;
    }

}
