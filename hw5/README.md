hw5.c

$ ls -1 | cut -d'.' -f1 | sort | uniq | tee dirents
yukarıdaki terminal komutlarını C programında exec ve pipe sistem çağrısıyla gerçekleştiren bir C programı
