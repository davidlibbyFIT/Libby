/*
 * Serial communications class. Does all communications on its own thread. 
 * Hides sync/async operation details.
 */

#ifndef SERIAL_H
#define SERIAL_H
#include "windows.h"

class SerialParams
{
public:
	SerialParams(); 	

	char _port[16];
	unsigned int _baud;	
	unsigned int _data_bits;
	unsigned int _stop_bits;
	char _parity;
};

class Serial
{
public:
	Serial();
	~Serial();

	bool setParams(const SerialParams *params);
	void getParams(SerialParams *params);

	bool isConnected();
	bool openComm();
	bool closeComm();

	bool write_sync(const unsigned char *data, unsigned long len);
	bool read_sync(unsigned char *data, unsigned long maxlen,
				unsigned long *bytes_read, unsigned char terminator);

	bool write_async(const unsigned char *data, unsigned long len);
	bool read_async(unsigned char *data, unsigned long maxlen,
				unsigned long *bytes_read, unsigned char terminator);

	bool cancel_async_io();
	bool io_complete();

private:
	HANDLE _hSerial;
	bool _io_pending;
	SerialParams _params;
};

#endif // ifndef SERIAL_H
