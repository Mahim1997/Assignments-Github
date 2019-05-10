 
package substitution;
 
public class CipherTextInfo {

    char cipheredLetter;
    int frequency;

    public CipherTextInfo(char cipheredLetter, int f) {
        this.cipheredLetter = cipheredLetter;
        this.frequency = f;
    }

    public char getCipheredLetter() {
        return cipheredLetter;
    }

    public void setCipheredLetter(char cipheredLetter) {
        this.cipheredLetter = cipheredLetter;
    }

    public int getFrequency() {
        return frequency;
    }

    public void setFrequency(int frequency) {
        this.frequency = frequency;
    }
    

}