#include "byte_stream.hh"

#include <algorithm>

// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */


using namespace std;

ByteStream::ByteStream(const size_t capa)
{

  // Initialising the variables
  maxcapacity = capa; 
  bits_read =0; 
  bits_written =0;
  input_end = false;

}

size_t ByteStream::write(const string &data) {
  
  if(input_end) return 0; 

  size_t val=data.size();

  if(data.size()>maxcapacity-bufferarray.size()) val=maxcapacity-bufferarray.size();

  for(int i=0; i<val; i++){
    bufferarray.push_back(data[i]);
  }

  bits_written+=val;

  return val;

}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
  
  string s;

  size_t peek_size=min((int)len, (int)bufferarray.size());

  for(int i=0; i<peek_size; i++){

    s=bufferarray[bufferarray.size()-1-i]+s;
  }

  return s;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
  if(len>bufferarray.size()) set_error();

  if(_error) return;
  
  string pop_string;
  size_t pop_size=min((int)len, (int)bufferarray.size());

  for(int i=0; i<pop_size; i++) {
    bufferarray.pop_front();
    bits_read++;
  }

}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
  
  //sets error
  if(len>bufferarray.size()) set_error();

  if(!_error){
    string read_string;

    read_string=peek_output(len);
    pop_output(len);

    return read_string;
  }

  return "";
}

void ByteStream::end_input() {input_end=true;}

bool ByteStream::input_ended() const { return input_end;}

size_t ByteStream::buffer_size() const {return bufferarray.size(); }

bool ByteStream::buffer_empty() const {return bufferarray.empty(); }

bool ByteStream::eof() const { return input_end && bufferarray.empty(); }

size_t ByteStream::bytes_written() const { return bits_written; }

size_t ByteStream::bytes_read() const { return bits_read;  }

size_t ByteStream::remaining_capacity() const { return maxcapacity-bufferarray.size(); }
