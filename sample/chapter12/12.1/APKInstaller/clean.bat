@echo Off
@echo 正在删除VS中间文件......
del /s /a *.obj *.ncb BuildLog.htm *.user *.exp *.pch *.idb *.suo *.res *.dep *.pdb Thumbs.db *.netmodule *.aps *.ilk 2>nul

@echo 清理工作完成了!
pause