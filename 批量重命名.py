import sys, string, os, shutil
#����Ŀ¼����ǰ׺����������������ƽṹ����prefix_0001
def RenameFiles(srcdir, prefix):
    srcfiles = os.listdir(srcdir)
    index = 1
    for srcfile in srcfiles:
        srcfilename = os.path.splitext(srcfile)[0][1:]
        sufix = os.path.splitext(srcfile)[1]
  #����Ŀ¼�¾�����ļ����޸�%�ź��ֵ��"%04d"���֧��9999
        destfile = srcdir + "//" + prefix + "%04d"%(index) + sufix
        srcfile = os.path.join(srcdir, srcfile)
        os.rename(srcfile, destfile)
        index += 1
srcdir = "D:/05Project/20150203����ǩ��ǽ/image"
prefix = "00"
RenameFiles(srcdir, prefix)
