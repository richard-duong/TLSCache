#ifndef __PACKET_H__
#define __PACKET_H__


class packet{

  private:
    char* msg;
    int len;

  public:
    packet():msg(nullptr), len(0){}

	// copy another packet
    packet(int len):len(len){
		this->msg = new char[this->len]{0};
	}

	// assume null terminated string
    packet(char* cpy_msg){
      this->len = strlen(cpy_msg);
      this->msg = new char[this->len]{0};
      strcpy(this->msg, cpy_msg);
    }

    // exact
    packet(char* cpy_msg, int cpy_len){
      len = cpy_len + 1;
      msg = new char[len];
      strncpy(msg, cpy_msg, cpy_len);
      msg[len - 1] = 0;
    } 


};

#endif
