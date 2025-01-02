#pragma once

#include <iostream>
#include <Windows.h>

using namespace std;

class Serial {
private:
	HANDLE hSerial;
public:
	void configure() {
		hSerial = CreateFile(
			L"COM5", // Replace with your COM port name
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED, // Enable overlapped I/O
			NULL);

		if (hSerial == INVALID_HANDLE_VALUE)
			std::cerr << "Error opening serial port" << endl;

		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		if (!GetCommState(hSerial, &dcbSerialParams))
			cerr << "Error getting serial port state" << endl;

		dcbSerialParams.BaudRate = CBR_9600;
		dcbSerialParams.ByteSize = 16;
		dcbSerialParams.StopBits = ONESTOPBIT;
		dcbSerialParams.Parity = NOPARITY;

		if (!SetCommState(hSerial, &dcbSerialParams))
			cerr << "Error setting serial port state" << endl;

		COMMTIMEOUTS timeouts;
		timeouts.ReadIntervalTimeout = 0;
		timeouts.ReadTotalTimeoutMultiplier = 0;
		timeouts.ReadTotalTimeoutConstant = 1;
		timeouts.WriteTotalTimeoutMultiplier = 0;
		timeouts.WriteTotalTimeoutConstant = 0;

		if (!SetCommTimeouts(hSerial, &timeouts)) {
			cerr << "Error setting timeouts" << endl;
		}
	}
	void write(string data) {
		OVERLAPPED ov = { 0 };
		ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // Event for async I/O

		DWORD bytesWritten;
		if (!WriteFile(hSerial, data.c_str(), data.length(), &bytesWritten, &ov)) {
			if (GetLastError() == ERROR_IO_PENDING) {
				WaitForSingleObject(ov.hEvent, INFINITE); // Wait for write to complete
				GetOverlappedResult(hSerial, &ov, &bytesWritten, TRUE);
			}
			else {
				std::cerr << "Error writing to serial port" << std::endl;
			}
		}
		else {
			std::cout << "Data written: " << data << std::endl;
		}
	}
	void write(short data) {
		OVERLAPPED ov = { 0 };
		ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // Event for async I/O

		DWORD bytesWritten;
		if (!WriteFile(hSerial, &data, sizeof(data), &bytesWritten, &ov)) {
			if (GetLastError() == ERROR_IO_PENDING) {
				WaitForSingleObject(ov.hEvent, INFINITE); // Wait for write to complete
				GetOverlappedResult(hSerial, &ov, &bytesWritten, TRUE);
			}
		}
	}

	std::string read() {
		OVERLAPPED ov = { 0 };
		ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // Event for async I/O

		char buffer[128];
		DWORD bytesRead;

		if (!ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, &ov)) {
			if (GetLastError() == ERROR_IO_PENDING) {
				WaitForSingleObject(ov.hEvent, INFINITE); // Wait for read to complete
				GetOverlappedResult(hSerial, &ov, &bytesRead, TRUE);
			}
			else {
				CloseHandle(ov.hEvent);
				return "";
			}
		}

		buffer[bytesRead] = '\0';
		CloseHandle(ov.hEvent); // Clean up event
		return std::string(buffer);
	}

};