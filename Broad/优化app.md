                byte[] compressOutput=ZLibUtils.compress(params[0]);
                int compresssize=compressOutput.length;
                
                //为FileKey和原始文件大小数据分配空间
                byte[] dst=new byte[compresssize+8];
                
                System.arraycopy(compressOutput,0, dst, 0, compresssize);
                int size=params[0].length;//原始文件大小  
                byte[] a=new byte[4];
                a[0] = (byte) (0xff & size);
                a[1] = (byte) ((0xff00 & size) >> 8);
                a[2] = (byte) ((0xff0000 & size) >> 16);
                a[3] = (byte) ((0xff000000 & size) >> 24);
                
                dst[compresssize+0]=FileKey[0];
                dst[compresssize+1]=FileKey[1];
                dst[compresssize+2]=FileKey[2];
                dst[compresssize+3]=FileKey[3];
                dst[compresssize+4]=a[0];
                dst[compresssize+5]=a[1];
                dst[compresssize+6]=a[2];
                dst[compresssize+7]=a[3];
                        
                fos.write(dst);
                fos.close();
                
                DLog.d(TAG,"compressOutput length:"+compressOutput.length);
                DLog.d(TAG,"params[0] length:"+params[0].length);
                DLog.d(TAG,"after append picture length:"+pictureFile.length());