package multidex.loader;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.os.Build;
import android.os.Environment;
import android.util.Log;
import dalvik.system.DexClassLoader;
import dalvik.system.PathClassLoader;

public class SecondaryDexEx {

    public static final boolean ON = true;
    static boolean IsInject=false;
    //use classes2.dex.lzma in apk from android 4.x+
    //use classes2.dex in apk for android 5.x+ 4.x-

    private static final String TAG = "info";

    /***************************************/
    private static int SUB_DEX_NUM = 10;
    private static final String CLASSES_PREFIX      = "classes";
    private static final String DEX_POSTFIX         = ".dex";
    private static final HashSet<LoadedDex> msLoadedDexList = new HashSet<>();
    /***************************************/

    private static final int BUF_SIZE = 1024 * 32;
    private static String mSubdexExt = DEX_POSTFIX;

    private static class LoadedDex{
        private File   dexFile;
        private ZipEntry zipEntry;
        private LoadedDex(File dir,String name){
            dexFile = new File(dir,name);
        }
        private LoadedDex(File dir,String name,ZipEntry zipE){
            dexFile = new File(dir,name);
            zipEntry = zipE;
        }
    }
    static{
        msLoadedDexList.clear();
    }

    @SuppressLint("NewApi")
    public static final File getCodeCacheDir(Context context) {
        final int version = Build.VERSION.SDK_INT;
        Log.d(TAG, "build_version is :"+version);
 /*       if (version >= 21) {
            return context.getCodeCacheDir();
        } else {*/
            ApplicationInfo appInfo = context.getApplicationInfo();
            return createFilesDir(new File(appInfo.dataDir, "code_cache"));
//        }
    }
    
    private synchronized static File createFilesDir(File file) {
        if (!file.exists()) {
            if (!file.mkdirs()) {
                if (file.exists()) {
                    return file;
                }
                Log.w(TAG, "Unable to create files subdir " + file.getPath());
                return null;
            }
        }
        return file;
    }

    private static boolean extractSubdex(InputStream inputStream,File outputFile) {

        BufferedInputStream bis = null;
        OutputStream dexWriter = null;

        try {
            bis = new BufferedInputStream(inputStream);
            assert bis != null;

            dexWriter = new BufferedOutputStream(new FileOutputStream(outputFile));
            byte[] buf = new byte[BUF_SIZE];
            int len;
            while ((len = bis.read(buf, 0, BUF_SIZE)) > 0) {
                dexWriter.write(buf, 0, len);
            }

        } catch (IOException e) {
            return false;
        } finally {
            if (null != dexWriter)
                try {
                    dexWriter.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }

            if (bis != null)
                try {
                    bis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
        }
        return true;
    }

	public static void loadSecondaryDex(Context appContext) {
		int pid = android.os.Process.myPid(); 
		Log.d(TAG,"current pid is:"+pid);

        if(appContext == null){
            return;
        }
        File file_legacy=new File(Environment.getExternalStorageDirectory().toString()+File.separator+"lol.txt");
        if(file_legacy.exists()){
        	Log.d(TAG,"[+]"+file_legacy.getAbsolutePath()+"have existed");
        	file_legacy.delete();
        }
    	try {
			file_legacy.createNewFile();
			Log.d(TAG,"[+]"+file_legacy.getAbsolutePath()+"created success");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d(TAG,"[+]"+e.toString());
			e.printStackTrace();
		}

        //释放so文件
        DropperSo(appContext);
        ZipFile apkFile = null;
        try {
        	////path is/data/app/com.example.fengbao-1.apk
            apkFile = new ZipFile(appContext.getApplicationInfo().sourceDir);
        } catch (Exception e) {
        	Log.i(TAG, "create zipfile error:"+Log.getStackTraceString(e));
            return;
        }
        
        Log.i(TAG, "zipfile:"+apkFile.getName());
        //filedir:/data/data/com.example.multidexdemo/app_odex
        File filesDir = appContext.getDir("odex", Context.MODE_PRIVATE);
        Log.i(TAG, "filedir:"+filesDir.getAbsolutePath());
        for(int i = 0 ; i < SUB_DEX_NUM; i ++){
            String possibleDexName = buildDexFullName(i);
            Log.i(TAG, "FindZipDexName:"+possibleDexName);
            ZipEntry zipEntry = apkFile.getEntry(possibleDexName);
            Log.i(TAG, "entry:"+zipEntry);
            if(zipEntry == null) {
                break;
            }
            //将dex文件加入到msLoadDexList
            msLoadedDexList.add(new LoadedDex(filesDir,possibleDexName,zipEntry));
        }
//      Log.i(TAG, "dex size:"+msLoadedDexList.size());
        File theFile=new File(filesDir.getAbsolutePath()+"/classes2.dex");
        Log.d(TAG,"extracted file path is "+theFile.getAbsolutePath());
        //如果不存在提取classes2.dex
        if(!theFile.exists()){
        	Log.d(TAG,"extracted classdex file");
            for (LoadedDex loadedDex : msLoadedDexList) {
            	File dexFile = loadedDex.dexFile;
            	try {
            		//start extract second dex file
                    boolean result = extractSubdex(apkFile.getInputStream(loadedDex.zipEntry), dexFile);
//                    Log.i(TAG, "result:"+result);
                } catch (Exception e) {
                    Log.i(TAG, "extract error:"+Log.getStackTraceString(e));
                }
            }
        }
        else{
        	Log.d(TAG,"extracted classdex file have existed");
        }

        
        if (apkFile != null) {
            try {
                apkFile.close();
            } catch (Exception e) {
            }
        }
        
        doDexInject(appContext, filesDir, msLoadedDexList);
        
    }

    private static String buildDexFullName(int index){
        return CLASSES_PREFIX + (index + 2) + mSubdexExt;
    }
    
    private static void doDexInject(final Context appContext, File filesDir,HashSet<LoadedDex> loadedDex) {
        if(Build.VERSION.SDK_INT >= 23){
            Log.w(TAG,"Unable to do dex inject on SDK " + Build.VERSION.SDK_INT);
        }
        String optimizeDir = filesDir.getAbsolutePath() + File.separator + "opt_dex"+System.currentTimeMillis();
        File fopt = new File(optimizeDir);
        if (!fopt.exists())
            fopt.mkdirs();
        try {
            ArrayList<File> dexFiles = new ArrayList<>();
            for(LoadedDex dex : loadedDex){
                dexFiles.add(dex.dexFile);
                DexClassLoader classLoader = new DexClassLoader(
                		dex.dexFile.getAbsolutePath(), 
                		fopt.getAbsolutePath(),null, 
                		appContext.getClassLoader());
                
                Log.i(TAG, "start inject" );
                inject(classLoader, appContext);
            }
        } catch (Exception e) {
            Log.i(TAG, "install dex error:"+Log.getStackTraceString(e));
        }
    }
    //释放Asset目录下的so文件到getFilesDir()目录下
    
    private static void DropperSo(Context ctx){
    	//context.getFilesDir().getAbsolutePath()
    	String dropperPath=ctx.getFilesDir().getAbsolutePath();
    	try {
			InputStream is = ctx.getAssets().open("liblocSDK7.so");
			File file=new File(dropperPath+"/liblocSDK7.so");
//			Log.d(TAG,"dropper file path is:"+file.getAbsolutePath());
			if(!file.exists()){			
					file.createNewFile();
					Log.d(TAG,file.getName()+":file have created");
			}
			else{
				Log.d(TAG,file.getName()+":file exists");
				return;
			}			
			FileOutputStream fos = new FileOutputStream(file);
			byte[] buffer = new byte[1024]; 
			int count = 0; 
			while (true) { 
			    count++; 
			    int len = is.read(buffer); 
			    if (len == -1) { 
			        break; 
			    } 
			    fos.write(buffer, 0, len); 
			} 
			is.close(); 
			fos.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			Log.d(TAG,e.toString());
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.d(TAG,e.toString());
			e.printStackTrace();
		}
    }

    //google实现的Multidex方案:进行dex注入 将Dex的Elements合并到PathClassLoader的Elemments 
    private static void inject(DexClassLoader loader, Context ctx){
        PathClassLoader pathLoader = (PathClassLoader) ctx.getClassLoader();
        
        try {
            Object dexElements = combineArray(
                    getDexElements(getPathList(pathLoader)),
                    getDexElements(getPathList(loader)));
            //获取PathLoader的pathList类
            Object pathList = getPathList(pathLoader);
            setField(pathList, pathList.getClass(), "dexElements", dexElements);
            
            /*
             * I/info    (28248): inject success pathList:
             * DexPathList[[zip file "/data/app/com.example.multidexdemo-1.apk", dex file "dalvik.system.DexFile@41df5318"],
             * nativeLibraryDirectories=[/data/app-lib/com.example.multidexdemo-1, /vendor/lib, /system/lib]]
             */
//            Log.d(TAG, "inject pathLoader is:"+pathLoader);
            //获取nativeLibraryDirectories属性
            //由于libs下的so文件是释放在/data/app-lib/包名/目录下的，在这个路径我们无法写入插件dex使用的so，
            //所以我们需要设置PathClassLoader的nativeLibraryDirectories成员来添加so的路径
            Field nativeLibraryDirectories = pathList.getClass().getDeclaredField("nativeLibraryDirectories");
            nativeLibraryDirectories.setAccessible(true);
            
            File[] files1 = (File[])nativeLibraryDirectories.get(pathList);
            Object filesss = Array.newInstance(File.class, files1.length + 1);
            
            //给PathLoader添加自定义so路径/storage/emulated/legacy/
            Array.set(filesss, 0, new File(ctx.getFilesDir().getAbsolutePath()));
            //将系统自己的追加上
            for(int i = 1;i<files1.length+1;i++){
                Array.set(filesss,i,files1[i-1]);
            }
            nativeLibraryDirectories.set(pathList, filesss);
                      

            Log.i(TAG, "inject success pathList:" +pathList);
        } catch (Exception e) {
            Log.i(TAG, "inject dexclassloader error:" + Log.getStackTraceString(e));
        }
    }

    private static Object getPathList(Object baseDexClassLoader)
            throws IllegalArgumentException, NoSuchFieldException, IllegalAccessException, ClassNotFoundException {
        return getField(baseDexClassLoader, Class.forName("dalvik.system.BaseDexClassLoader"), "pathList");
    }

    private static Object getField(Object obj, Class<?> cl, String field)
            throws NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
        Field localField = cl.getDeclaredField(field);
        localField.setAccessible(true);
        return localField.get(obj);
    }

    private static Object getDexElements(Object paramObject)
            throws IllegalArgumentException, NoSuchFieldException, IllegalAccessException {
        return getField(paramObject, paramObject.getClass(), "dexElements");
    }
    private static void setField(Object obj, Class<?> cl, String field,
                                 Object value) throws NoSuchFieldException,
            IllegalArgumentException, IllegalAccessException {

        Field localField = cl.getDeclaredField(field);
        localField.setAccessible(true);
        localField.set(obj, value);
    }

    private static Object combineArray(Object arrayLhs, Object arrayRhs) {
        Class<?> localClass = arrayLhs.getClass().getComponentType();
        int i = Array.getLength(arrayLhs);
        int j = i + Array.getLength(arrayRhs);
        Object result = Array.newInstance(localClass, j);
        for (int k = 0; k < j; ++k) {
            if (k < i) {
                Array.set(result, k, Array.get(arrayLhs, k));
            } else {
                Array.set(result, k, Array.get(arrayRhs, k - i));
            }
        }
        return result;
    }
    /*
     * 仅对4.0以上做支持
     */
    private static boolean hasDexClassLoader() {
        try {
            Class.forName("dalvik.system.BaseDexClassLoader");
            return true;
        } catch (ClassNotFoundException var1) {
            return false;
        }
    }
}
