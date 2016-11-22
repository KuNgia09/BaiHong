# java AES加解密

## 当初是为了解决l-ctf 2016 android第一题学习如何使用java AES加解密

```java
package aes;
import java.security.InvalidKeyException;
import java.security.Key;
import java.security.NoSuchAlgorithmException;
import javax.crypto.*;
import javax.crypto.spec.*;
/**
*
* @author wchun
*
* AES128 算法，加密模式为ECB，填充模式为 pkcs7（实际就是pkcs5）
*
*
*/
public class AES {
static final String algorithmStr="AES/ECB/PKCS5Padding";
static private KeyGenerator keyGen;
static private Cipher cipher;
static boolean isInited=false;
//初始化
static private void init()
{
//初始化keyGen
    try {
        keyGen=KeyGenerator.getInstance("AES");
    } catch (NoSuchAlgorithmException e) {
    // TODO Auto-generated catch block
        e.printStackTrace();
    }
        keyGen.init(128);
    //初始化cipher
    try {
    cipher=Cipher.getInstance(algorithmStr);
    } catch (NoSuchAlgorithmException e) {
    // TODO Auto-generated catch block
        e.printStackTrace();
    } catch (NoSuchPaddingException e) {
    // TODO Auto-generated catch block
        e.printStackTrace();
    }
        isInited=true;
    }
    public static byte[] GenKey()
    {
        if(!isInited)//如果没有初始化过,则初始化
        {
            init();
        }
    return keyGen.generateKey().getEncoded();
}

public static byte[] Encrypt(byte[] content,byte[] keyBytes)
{
    byte[] encryptedText=null;
    if(!isInited)//为初始化
    {
        init();
    }
    Key key=new SecretKeySpec(keyBytes,"AES");
    try {
        cipher.init(Cipher.ENCRYPT_MODE, key);
    } catch (InvalidKeyException e) {
    // TODO Auto-generated catch block
        e.printStackTrace();
    }
    try {
        encryptedText=cipher.doFinal(content);
    } catch (IllegalBlockSizeException e) {
    // TODO Auto-generated catch block
        e.printStackTrace();
    } catch (BadPaddingException e) {
    // TODO Auto-generated catch block
        e.printStackTrace();
    }
    return encryptedText;
}
//解密为byte[]
public static byte[] DecryptToBytes(byte[] content,byte[] keyBytes)
{
    byte[] originBytes=null;
    if(!isInited)
    {
        init();
    }

    Key key=new SecretKeySpec(keyBytes,"AES");
    try {
        cipher.init(Cipher.DECRYPT_MODE, key);
    } catch (InvalidKeyException e) {
    // TODO Auto-generated catch block
    e.printStackTrace();
    }
    //解密
    try {
        originBytes=cipher.doFinal(content);
    } catch (IllegalBlockSizeException e) {
    // TODO Auto-generated catch block
        e.printStackTrace();
    } catch (BadPaddingException e) {
    // TODO Auto-generated catch block
        e.printStackTrace();
    }
    return originBytes;
}

/**
 *   EncodeData:AES加密之后密文
 *   key:AES秘钥
 *   MingWen:加密前明文
*/
static byte[] EncodeData={21, -93, -68, -94, 86, 117, -19, -68,
                        -92, 33, 50, 118, 16, 13, 1, -15,
                        -13, 3, 4, 103, -18, 81, 30, 68,
                        54, -93, 44, -23, 93, 98, 5, 59};
static String key="htsii__sht_eek.y";
static String MingWen="LCTF{1t's_rea1ly_an_ea3y_ap4}";

public static void main(String[] args){
    byte[] Encode=Encrypt(MingWen.getBytes(),key.getBytes());
    byte[] Decode=DecryptToBytes(EncodeData,key.getBytes());
    System.out.println("flag is:"+new String(Decode));
    for(int i=0;i<Encode.length;i++){
        System.out.println("Encode data is :"+Encode[i]);
    }
}

}
```
