# gsl_regression_test
test non-linear regression using gsl


To compile:

clang++ -w -o ./gsl_example -O4 ./gsl_example.cpp -I /usr/local/include/ -L /usr/local/lib/ -lgsl -lgslcblas

To run:

./gsl_example

