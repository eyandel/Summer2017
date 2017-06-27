int myMacro(int k=0){
	cout << "Trivial ROOT macro: Hello, World!" << endl;
	// it sounds like "Cognito, ergo sum" said by HAL9000...
	cout<< "The input parameter was " << k << endl;
	return k;
}

double anotherFunc(int j=0){
 
  double pp = 2*acos(-1);
  double x = 1.5*j + pp*myMacro(j);
  return x;
 
}