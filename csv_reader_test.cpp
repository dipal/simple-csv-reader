
#include "csv_reader.hpp"

#define Log(X) std::cout << X << std::endl;

template<typename T>
void printVector(std::vector<T> v) {
	for (int i=0; i<v.size(); i++) {
		if (i) {
			std::cout << ", ";
		}

		std::cout << v[i];
	}

	std::cout << std::endl;
}

template<typename T>
void printVector2d(std::vector<T> v) {
	for (int i=0; i<v.size(); i++) {
		printVector(v[i]);
	}

	std::cout << std::endl;
}


int main()
{
	std::string csv_file = "b.csv";
	char delimeter = ',';
	bool isHeader = true;

	Log("");
	Log("dcsv::Reader");
	Log("csv_file: " << csv_file << " delimeter: '" << delimeter << "' isHeader: " << isHeader);
	Log("");

	dcsv::Reader csvReader(csv_file, delimeter, isHeader);
	csvReader.readAll();

	Log("Reader::toString()")
	Log(csvReader.toString());

	Log("Reader::getAsArray<int>()");
	printVector2d(csvReader.getAsArray<int>());

	Log("Reader::getAsArray<double>()");
	printVector2d(csvReader.getAsArray<double>());
	
	Log("Reader::getAsArray<std::string>()");
	printVector2d(csvReader.getAsArray<std::string>());

	Log("Reader::getAsArray<long>()");
	printVector2d(csvReader.getAsArray<long>());
	
	Log("Reader::getHeaders()");
	printVector(csvReader.getHeaders());
	Log("")

	Log("Reader::getDataByRowIndex<int>(1)");
	printVector(csvReader.getDataByRowIndex<int>(1));
	Log("")

	Log("Reader::getDataByColumnIndex<int>(1)");
	printVector(csvReader.getDataByColumnIndex<int>(1));
	Log("")

	Log("Reader::getDataByColumnName<int>(\"c\")");
	printVector(csvReader.getDataByColumnName<int>("c"));
	Log("")

	// auto data = csvReader.getAsArray();
	// for (int i=0; i<data.size(); i++) {
	// 	for (int j=0; j<data[i].size(); j++) {
	// 		std::cout << data[i][j] << " ";
	// 	}

	// 	std::cout << std::endl;
	// }

	return 0;
}
