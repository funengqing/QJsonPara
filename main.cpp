#include <QCoreApplication>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
using namespace std;


void trim(string& s){
  size_t index = 0;
  while (true) {
      index = s.find(" ");
      if(index == string::npos){
          return;
        }
      s.erase(index,1);
    }
}

void oFile(string tag,string big, string small, string name){
  ofstream ofs;

  ofs.open("/home/neng/LearnCpp/CppInQt/fileOper/mysqlup57.sh",ios::out|ios::app);
  string add0 = "ffmpeg -i ";
  //string add1 = "ftp://neng:neng@192.168.123.157:9999/tv.danmaku.bili/download/";//远程用
  //有线用
  string add1 = "/run/user/1000/gvfs/mtp:host=Meizu_PRO_6_Plus_M96GBEQA225NQ/内部共享存储空间/1/tv.danmaku.bili/download/";
  string add2 = big;
  add2 += "/c_";
  string add3 = small;
  string add4 = "/"+tag+"/video.m4s -i ";
  string add5 = "/"+tag+"/audio.m4s ";
  string add6 = "-codec copy ";

  //3 照片后的名字要改成目标文件夹的名字

  string tagDir = "mysqlup57";
  tagDir += "/";

  //cout << tagDir << endl;
  //pause();
  string add7 = "/run/user/1000/gvfs/smb-share:server=192.168.123.1,share=照片/"+tagDir;
  string add8 = name;
  add8+= ".mp4";
  string content = add0+add1+add2+add3+add4+add1+add2+add3+add5+add6+add7+add8+"\n";
  ofs<<content;
  ofs.close();
};

void funjson(){
  //1 这里要改成每个视频的
  int idArr[] = {281506819,281508150,281544509,281545552,281546659,281507443,281508169,281544536,281545631,281546830,281507813,281508276,281544857,281546102,281546869,281507861,281544386,281544927,281546307,281547130,281508045,281544500,281544996,281546324,281547293};

  //cout << sizeof (idArr)/sizeof (int) << endl;
  //2 这里要改成最外层的视频项目编号
  string big = to_string(203708726);

  int len = sizeof (idArr)/sizeof (int);
  for (int i=0; i<len; i++ ) {
      string str_addre = "c_" + to_string(idArr[i]);
      QString addre = QString::fromStdString(str_addre);

      QFile file("/run/user/1000/gvfs/mtp:host=Meizu_PRO_6_Plus_M96GBEQA225NQ/内部共享存储空间/1/tv.danmaku.bili/download/"+QString::fromStdString(big)+"/"+addre+"/entry.json");
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      QString value = file.readAll();
      file.close();

      QJsonParseError parseJsonErr;
      QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
      if (! (parseJsonErr.error == QJsonParseError::NoError)) {
          cout << "error" << endl;
          return;
      }
      //解析最外层的大括号,请注意是QJsonObject
      QJsonObject jsonObject = document.object();
      QString tag = jsonObject["type_tag"].toString();
      //cout << tag.toStdString()<< endl;

      //解析字段中值是对象,请注意也是个QJsonObject.看是64还是80
      QJsonValue jsonDataValue = jsonObject.value(QStringLiteral("page_data"));
      QJsonObject jsonData = jsonDataValue.toObject();

      int page = jsonData["page"].toInt();

      QString tempPart = jsonData["part"].toString();
      string part = tempPart.toStdString();

//      size_t index = part.find(" ");
//      if(index != -1){
//         part.erase(index,1);
//        }

      //去掉所有空格
      trim(part);
      cout << page<< "_" << part << endl;

      //写到文件上
      string small = to_string(idArr[i]);//每个视频
      string name = to_string(page)+"_"+part;//名字

      oFile(tag.toStdString(),big,small,name);
    }
}



int main(int argc, char *argv[])
{

  QCoreApplication a(argc, argv);
  funjson();
  return a.exec();
}
