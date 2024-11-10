#include <iostream>
#include "leveldb/include/leveldb/db.h"
using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(){
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;

  leveldb::Status status = leveldb::DB::Open(options, "./mydb", &db);

  if (!status.ok()){
    cerr << "Unable to open/create database 'mydb'" << status.ToString() << endl;
    return -1;
  }

string key = "yujeong";
string value = "Seoul";

status = db->Put(leveldb::WriteOptions(), key, value);
if (!status.ok()){
  cerr << "Failed to write key and value" << status.ToString() << endl;
}

string read_value;
status = db->Get(leveldb::ReadOptions(), key, &read_value);
if (!status.ok()){
  cerr << "Failed to get" << status.ToString() << endl;
}
else{
  cout << "Read value = " << read_value << endl;
}

status = db->Delete(leveldb::WriteOptions(), key);
if (!status.ok()){
  cerr << "Failed to delete" << status.ToString() << endl;
}

return 0;
}