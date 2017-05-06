#include "matrix/Matrix.hpp"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/test/unit_test.hpp>

using namespace util::matrix;

BOOST_AUTO_TEST_SUITE(MatrixTest)

BOOST_AUTO_TEST_CASE(DefaultConstruct) {
    Matrix<int> matrix{};
    BOOST_TEST(matrix.width() == 0);
    BOOST_TEST(matrix.height() == 0);
}

BOOST_AUTO_TEST_CASE(ConstructWithSize) {
    Matrix<int> matrix{2, 3};
    BOOST_TEST(matrix.width() == 2);
    BOOST_TEST(matrix.height() == 3);
}

BOOST_AUTO_TEST_CASE(ConstructWithDefaultValue) {
    const int value = 423;
    Matrix<int> matrix{3, 2, value};
    BOOST_TEST(matrix.width() == 3);
    BOOST_TEST(matrix.height() == 2);
    BOOST_CHECK_EQUAL((matrix[Point{0, 0}]), value);
    BOOST_CHECK_EQUAL((matrix[Point{1, 0}]), value);
    BOOST_CHECK_EQUAL((matrix[Point{2, 0}]), value);
    BOOST_CHECK_EQUAL((matrix[Point{0, 1}]), value);
    BOOST_CHECK_EQUAL((matrix[Point{1, 1}]), value);
    BOOST_CHECK_EQUAL((matrix[Point{2, 1}]), value);
}

BOOST_AUTO_TEST_CASE(SetValue) {
    Matrix<int> matrix{2, 2};
    matrix[Point{0, 0}] = 1;
    matrix[Point{1, 0}] = 2;
    matrix[Point{0, 1}] = 3;
    matrix[Point{1, 1}] = 4;
    BOOST_CHECK_EQUAL((matrix[Point{0, 0}]), 1);
    BOOST_CHECK_EQUAL((matrix[Point{1, 0}]), 2);
    BOOST_CHECK_EQUAL((matrix[Point{0, 1}]), 3);
    BOOST_CHECK_EQUAL((matrix[Point{1, 1}]), 4);
}

BOOST_AUTO_TEST_CASE(AccessByIndex) {
    Matrix<int> matrix{2, 2};
    for (std::size_t i = 0; i < 4; ++i) {
        matrix[i] = i;
    }
    for (std::size_t i = 0; i < 4; ++i) {
        BOOST_TEST(matrix[i] == i);
    }
    BOOST_CHECK_EQUAL((matrix[Point{0, 0}]), 0);
    BOOST_CHECK_EQUAL((matrix[Point{1, 0}]), 1);
    BOOST_CHECK_EQUAL((matrix[Point{0, 1}]), 2);
    BOOST_CHECK_EQUAL((matrix[Point{1, 1}]), 3);
}

BOOST_AUTO_TEST_CASE(Iterate) {
    Matrix<int> matrix{2, 2};
    int i = 10;
    for (int& value : matrix) {
        value = i++;
    }
    i = 10;
    for (int value : matrix) {
        BOOST_TEST(value == i++);
    }
    BOOST_CHECK_EQUAL((matrix[Point{0, 0}]), 10);
    BOOST_CHECK_EQUAL((matrix[Point{1, 0}]), 11);
    BOOST_CHECK_EQUAL((matrix[Point{0, 1}]), 12);
    BOOST_CHECK_EQUAL((matrix[Point{1, 1}]), 13);
}

BOOST_AUTO_TEST_CASE(Reset) {
    Matrix<int> matrix{2, 3};
    matrix.reset(6, 8);
    BOOST_TEST(matrix.width() == 6);
    BOOST_TEST(matrix.height() == 8);
}

BOOST_AUTO_TEST_CASE(ResetWithDefaultValue) {
    int value = 56;
    Matrix<int> matrix{20, 30};
    matrix.reset(2, 1, value);
    BOOST_TEST(matrix.width() == 2);
    BOOST_TEST(matrix.height() == 1);
    BOOST_CHECK_EQUAL((matrix[Point{0, 0}]), value);
    BOOST_CHECK_EQUAL((matrix[Point{1, 0}]), value);
}

BOOST_AUTO_TEST_CASE(Fill) {
    int value = 5;
    Matrix<int> matrix{1, 3};
    matrix.fill(value);
    BOOST_CHECK_EQUAL((matrix[Point{0, 0}]), value);
    BOOST_CHECK_EQUAL((matrix[Point{0, 1}]), value);
    BOOST_CHECK_EQUAL((matrix[Point{0, 2}]), value);
}

BOOST_AUTO_TEST_CASE(EqualMatrices) {
    Matrix<int> matrix1{2, 3};
    Matrix<int> matrix2{2, 3};
    for (std::size_t i = 0; i < 6; ++i) {
        matrix1[i] = i;
        matrix2[i] = i;
    }
    BOOST_CHECK(matrix1 == matrix2);
    BOOST_CHECK(!(matrix1 != matrix2));
}

BOOST_AUTO_TEST_CASE(DifferentMatrices) {
    Matrix<int> matrix1{2, 3};
    Matrix<int> matrix2{2, 3};
    for (std::size_t i = 0; i < 6; ++i) {
        matrix1[i] = i;
        matrix2[i] = i + 1;
    }
    BOOST_CHECK(!(matrix1 == matrix2));
    BOOST_CHECK(matrix1 != matrix2);
}

BOOST_AUTO_TEST_CASE(DifferentSizedMatrices) {
    Matrix<int> matrix1{2, 3};
    Matrix<int> matrix2{3, 2};
    for (std::size_t i = 0; i < 6; ++i) {
        matrix1[i] = i;
        matrix2[i] = i;
    }
    BOOST_CHECK(!(matrix1 == matrix2));
    BOOST_CHECK(matrix1 != matrix2);
}

BOOST_AUTO_TEST_CASE(Serialize) {
    Matrix<int> matrix1{2, 3};
    for (std::size_t i = 0; i < 6; ++i) {
        matrix1[i] = i;
    }
    std::stringstream ss;
    {
        boost::archive::text_oarchive ar{ss};
        ar << matrix1;
    }
    ss.seekg(0);
    Matrix<int> matrix2;
    {
        boost::archive::text_iarchive ar{ss};
        ar >> matrix2;
    }
    BOOST_CHECK_EQUAL_COLLECTIONS(matrix1.begin(), matrix1.end(),
            matrix2.begin(), matrix2.end());
}

BOOST_AUTO_TEST_CASE(Copy) {
    Matrix<int> matrix1{2, 3};
    for (std::size_t i = 0; i < 6; ++i) {
        matrix1[i] = i;
    }
    auto matrix2 = matrix1;
    BOOST_CHECK_EQUAL_COLLECTIONS(matrix1.begin(), matrix1.end(),
            matrix2.begin(), matrix2.end());
}

BOOST_AUTO_TEST_CASE(Move) {
    Matrix<int> matrix1{2, 3};
    Matrix<int> matrix2{2, 3};
    for (std::size_t i = 0; i < 6; ++i) {
        matrix1[i] = i;
        matrix2[i] = i;
    }
    auto matrix3 = std::move(matrix1);
    BOOST_TEST(matrix1.width() == 0);
    BOOST_TEST(matrix1.height() == 0);
    BOOST_CHECK_EQUAL_COLLECTIONS(matrix3.begin(), matrix3.end(),
            matrix2.begin(), matrix2.end());
}

BOOST_AUTO_TEST_CASE(BoolMatrix) {
    Matrix<bool> matrix{2, 2};
    matrix[Point{0, 0}] = true;
    matrix[Point{1, 0}] = false;
    matrix[Point{0, 1}] = true;
    matrix[Point{1, 1}] = true;
    BOOST_CHECK_EQUAL((matrix[Point{0, 0}]), true);
    BOOST_CHECK_EQUAL((matrix[Point{1, 0}]), false);
    BOOST_CHECK_EQUAL((matrix[Point{0, 1}]), true);
    BOOST_CHECK_EQUAL((matrix[Point{1, 1}]), true);
}

BOOST_AUTO_TEST_SUITE_END() // MatrixTest