/*
 * Serial communications class. Does all communications on its own thread. 
 * Hides sync/async operation details.
 */

#include "StdAfx.h"
#include "Serial.h"


SerialParams::SerialParams() 
	: _baud(9600), _data_bits(8), _stop_bits(1), _parity('N') 
{
	strncpy_s(_port, sizeof(_port), "COM1", _TRUNCATE);
}


Serial::Serial() : _hSerial(0), _io_pending(false)
{
}

Serial::~Serial()
{
	if (_hSerial)
		closeComm();
}

bool Serial::setParams(const SerialParams *params)
{
	if (!params)
		return false;

	switch (params->_baud) {
	case CBR_110:
	case CBR_300:
	case CBR_600:
	case CBR_1200:
	case CBR_2400:
	case CBR_4800:
	case CBR_9600:
	case CBR_14400:
	case CBR_19200:
	case CBR_38400:
	case CBR_56000:
	case CBR_57600:
	case CBR_115200:
	case CBR_128000:
	case CBR_256000:
		break;

	default:
		// needs to be one of these
		return false;
	}

	if (strlen(params->_port) < 4) {
		return false;
	}

	_params._baud = params->_baud;
	strncpy_s(_params._port, sizeof(_params._port), params->_port, _TRUNCATE);

	return true;
}

void Serial::getParams(SerialParams *params)
{
	if (params)
		memcpy(params, &_params, sizeof(_params));
}

bool Serial::isConnected()
{
	return (_hSerial != 0);
}

bool Serial::openComm()
{
	DCB dcb;
	COMMTIMEOUTS timeouts;

	if (isConnected())
		return true;

	_hSerial = CreateFile(_params._port, 
						GENERIC_READ | GENERIC_WRITE,
						0,
						0,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						0);

	if (INVALID_HANDLE_VALUE == _hSerial) {

#if !defined STANDALONE_PUMPGUI
		char buff[100];
		char sysmsg[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, sysmsg, sizeof(sysmsg) - 1, NULL);
		if (strlen(sysmsg) >= 2)
			sysmsg[strlen(sysmsg) - 2] = 0;
		sprintf_s(buff, "INVALID_FILE_HANDLE in openComm(%s): %s", _params._port, sysmsg);	                            
#endif

#if defined STANDALONE_PUMPGUI
		if (ERROR_FILE_NOT_FOUND == GetLastError()) {
			MessageBox(NULL, "Serial port not found", "Error", MB_OK);
		}
		else {
			MessageBox(NULL, "Error opening serial port", "Error", MB_OK);
		}
#endif
		_hSerial = 0;
		return false;
	}

	memset(&dcb, 0, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);
	dcb.fBinary = 1;
	dcb.BaudRate = _params._baud;
	dcb.ByteSize = _params._data_bits;
	dcb.StopBits = (_params._stop_bits == 2) ? TWOSTOPBITS : ONESTOPBIT;
	
	switch (_params._parity) {
	case 'N':
		dcb.Parity = NOPARITY;
		break;
	case 'E':
		dcb.Parity = EVENPARITY;
		break;
	case 'O':
		dcb.Parity = ODDPARITY;
		break;
	case 'M':
		dcb.Parity = MARKPARITY;
		break;
	case 'S':
		dcb.Parity = SPACEPARITY;
		break;
	}
	
	if (!SetCommState(_hSerial, &dcb)) {
		closeComm();
		return false;
	}

	memset(&timeouts, 0, sizeof(timeouts));
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 100;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 100;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(_hSerial, &timeouts)) {
		closeComm();
		return false;
	}

	return true;
}

bool Serial::closeComm()
{
	if (isConnected()) {
		if (_io_pending) {
			cancel_async_io();
		}

		CloseHandle(_hSerial);
		_hSerial = 0;
	}

	return true;
}

bool Serial::write_sync(const unsigned char *data, unsigned long len)
{
	unsigned long bytes_written = 0;
	unsigned long total_bytes_written = 0;
	
	if (!isConnected())
		return false;

	if(data[0]=='/')
		int aa=1;

	while (total_bytes_written < len) {
		if (!WriteFile(_hSerial, 
						&data[total_bytes_written], 
						len - total_bytes_written, 
						&bytes_written, 
						NULL)) {
			// some error here
			break;
		}

		total_bytes_written += bytes_written;
	}

	return (total_bytes_written == len);
}

bool Serial::read_sync(unsigned char *data, unsigned long maxlen, 
					   unsigned long *bytes_read, unsigned char terminator)
{
	unsigned long count = 0;
	unsigned long total_count = 0;
	bool done = false;
	int loops = 0;

	if (!isConnected())
		return false;

	while (!done && total_count < maxlen) {
		if (!ReadFile(_hSerial, 
						&data[total_count], 
						maxlen - total_count, 
						&count, 
						NULL)) {
			// some error here
			break;
		}

		for (unsigned long i = total_count; i < total_count + count; i++) {
			if (data[i] == terminator) {
				done  = true;
				break;
			}
		}

		total_count += count;

		if (total_count >= maxlen) {
			done = true;
			break;
		}

		if (loops++ > 4)
			break;

		Sleep(250);
	}

	if (bytes_read)
		*bytes_read = total_count;

	return done;
}

bool Serial::write_async(const unsigned char *data, unsigned long len)
{
	return false;
}

bool Serial::read_async(unsigned char *data, unsigned long maxlen,
						unsigned long *bytes_read, unsigned char terminator)
{
	return false;
}

bool Serial::cancel_async_io()
{
	return false;
}

bool Serial::io_complete()
{
	return false;
}
