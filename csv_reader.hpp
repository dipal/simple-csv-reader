#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <map>
#include <type_traits>

namespace dcsv {

	template <typename T>
	T convert(std::string v) {
		if (std::is_same<T, int>::value) {
			return std::stoi(v, nullptr);
		}

		if (std::is_same<T, double>::value) {
			return std::stod(v, nullptr);
		}

		return T();
	}

	template <>
	std::string convert<std::string> (std::string v) {
		return v;
	}

	class Reader {
		std::string filename;
		bool isHeader;
		bool isTrimData;
		char delimeter;
		bool isRead;
		std::vector<std::vector<std::string> > data;
		std::vector<std::string> headers;
		std::map<std::string, int> headerToColumn;

		std::string trim(std::string tdata);

		std::vector<std::string> parseLine(std::string &line, bool needToTrim);

		void parseHeader(std::string &line);

		void parseData(std::string &line);

		void readAndParse();

	public:
		Reader(std::string filename, char delimeter = '\t', bool isHeader = true, bool isTrimData = true);
		
		void readAll();

		template <typename T>
		T getData(int r, int c);

		template <typename T>
		std::vector<std::vector<T> > getAsArray();

		std::vector<std::string> getHeaders();

		template<typename T>
		std::vector<T> getDataByColumnIndex(int columnIndex);

		template<typename T>
		std::vector<T> getDataByColumnName(std::string columnName);

		template<typename T>
		std::vector<T> getDataByRowIndex(int rowIndex);
		
		std::string toString();
	};


	std::string Reader::trim(std::string tdata) {
		std::string trimmedData;
		int s = 0;
		while (tdata[s] == ' ' || tdata[s] == '\t') s++;

		int e = tdata.size() - 1;
		while (e > s && (tdata[e] == ' ' || tdata[e] == '\t')) e--;

		for (int i=s; i<=e; i++) {
			trimmedData.push_back(tdata[i]);
		}

		return trimmedData;
	}

	std::vector<std::string> Reader::parseLine(std::string &line, bool needToTrim) {
		if (line.size() == 0) {
			return std::vector<std::string>();
		}

		std::vector<std::string> lineData;
		std::string temp;
		bool isInQuote = false;
		for (int i = 0; i<line.size(); i++) {
			if (line[i] == '\"' || line[i] == '\'') {
				isInQuote = !isInQuote;
			} else if (line[i] == '\\') {
				if (i + 1 < line.size()) {
					temp.push_back(line[i+1]);
					i++;
				}
			} else if (line[i] == delimeter && !isInQuote) {
				if (needToTrim) {
					temp = trim(temp);
				}
				lineData.push_back(temp);
				temp = "";
			} else {
				temp.push_back(line[i]);
			}
		}

		if (needToTrim) {
			temp = trim(temp);
		}
		lineData.push_back(temp);

		return lineData;
	}

	void Reader::parseHeader(std::string &line) {
		auto lineData = parseLine(line, true);
		for (int i=0; i<lineData.size(); i++) {
			headers.push_back(lineData[i]);
			headerToColumn[lineData[i]] = i;
		}
	}

	void Reader::parseData(std::string &line) {
		auto lineData = parseLine(line, isTrimData);
		if (lineData.size() == 0) {
			return ;
		}

		data.push_back(std::move(lineData));
	}

	void Reader::readAndParse() {
		if (isRead) {
			return;
		}

		std::ifstream ifs(filename);
		std::string s;

		if (isHeader) {
			getline(ifs, s);
			parseHeader(s);
		}

		while (getline(ifs, s)) {
			parseData(s);
		}

		isRead = true;
	}

	Reader::Reader(std::string filename, char delimeter, bool isHeader, bool isTrimData) {
		this->filename = filename;
		this->delimeter = delimeter;
		this->isHeader = isHeader;
		this->isTrimData = isTrimData;
		this->isRead = false;
	}

	void Reader::readAll() {
		readAndParse();
	}

	template <typename T>
	T Reader::getData(int r, int c) {
		readAll();
		if (r < 0 || r >= data.size()) return T();
		if (c < 0 || c >= data[r].size()) return T();

		return convert<T>(data[r][c]);
	}

	template <typename T>
	std::vector<std::vector<T> > Reader::getAsArray() {
		readAll();
		std::vector<std::vector<T> > ret;
		for (int i=0; i<data.size(); i++) {
			std::vector<T> trow;
			for (int j=0; j<data[i].size(); j++) {
				trow.push_back(getData<T>(i, j));
			}

			ret.push_back(std::move(trow));
		}

		return ret;
	}

	std::vector<std::string> Reader::getHeaders() {
		readAll();
		return headers;
	}

	template<typename T>
	std::vector<T> Reader::getDataByColumnIndex(int columnIndex) {
		readAll();
		std::vector<T> ret;
		for (int i=0; i<data.size(); i++) {
			if (columnIndex < data[i].size()) {
				ret.push_back(getData<T>(i, columnIndex));
			}
		}

		return ret;
	}

	template<typename T>
	std::vector<T> Reader::getDataByColumnName(std::string columnName) {
		readAll();
		if (headerToColumn.find(columnName) != headerToColumn.end()) {
			int columnIndex = headerToColumn[columnName];
			return getDataByColumnIndex<T>(columnIndex);
		}

		return std::vector<T>();
	}

	template<typename T>
	std::vector<T> Reader::getDataByRowIndex(int rowIndex) {
		readAll();
		std::vector<T> ret;
		if (rowIndex >= data.size()) {
			return ret;
		}

		for (int j=0; j<data[rowIndex].size(); j++) {
			ret.push_back(getData<T>(rowIndex, j));
		}

		return ret;
	}

	std::string Reader::toString() {
		readAll();
		std::string csvStr;
		if (isHeader) {
			for (int i=0; i<headers.size(); i++) {
				if (i) {
					csvStr.push_back('|');
				}
				csvStr += headers[i];
			}
			csvStr.push_back('\n');
		}

		for (int i=0; i<data.size(); i++) {
			for (int j=0; j<data[i].size(); j++) {
				if (j) {
					csvStr.push_back('|');
				}
				csvStr += data[i][j];
			}
			csvStr.push_back('\n');
		}

		return csvStr;
	}
}

