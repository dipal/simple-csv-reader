
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
	std::cout << "===>>S<<===" << std::endl;
	for (int i=0; i<v.size(); i++) {
		printVector(v[i]);
	}

	std::cout << "===>>E<<===" << std::endl;

	std::cout << std::endl;
}


int main()
{
	dcsv::Reader csvReader("b.csv", ',', true);
	csvReader.readAll();

	Log(csvReader.toString());
	printVector2d(csvReader.getAsArray<int>());
	// printVector(csvReader.getAsArray<double>());
	// printVector(csvReader.getAsArray<std::string>());

	printVector(csvReader.getHeaders());
	printVector(csvReader.getDataByRowIndex<int>(1));
	printVector(csvReader.getDataByColumnIndex<int>(1));
	printVector(csvReader.getDataByColumnName<int>("c"));

	// auto data = csvReader.getAsArray();
	// for (int i=0; i<data.size(); i++) {
	// 	for (int j=0; j<data[i].size(); j++) {
	// 		std::cout << data[i][j] << " ";
	// 	}

	// 	std::cout << std::endl;
	// }

	return 0;
}
