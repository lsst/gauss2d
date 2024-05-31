#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include <memory>

#include "lsst/gauss2d/image.h"
#include "lsst/gauss2d/vectorimage.h"

namespace g2 = lsst::gauss2d;

typedef g2::VectorImage<double> Image;
typedef g2::ImageArray<double, Image> ImageArray;
typedef g2::VectorImage<bool> Mask;

TEST_CASE("VectorImage") {
    size_t n_rows = 3, n_cols = 2;
    std::shared_ptr<Image> image = std::make_shared<Image>(n_rows, n_cols);
    Image zeros{n_rows, n_cols};
    auto x = Image(n_rows, n_cols) == zeros;

    CHECK(x);

    CHECK_EQ(image->get_coordsys(), g2::COORDS_DEFAULT);
    CHECK_EQ(&(image->get_coordsys()), &(g2::COORDS_DEFAULT));

    CHECK_EQ(image->get_n_cols(), n_cols);
    CHECK_EQ(image->get_n_rows(), n_rows);
    CHECK_EQ(image->size(), n_cols * n_rows);

    image->add_value_unchecked(0, 0, 1);
    CHECK_NE(*image, zeros);
    CHECK_EQ(image->get_value_unchecked(0, 0), 1);
    CHECK_THROWS_AS(image->get_value(n_rows, n_cols), std::out_of_range);
    CHECK_THROWS_AS(image->set_value(n_rows, n_cols, 1), std::out_of_range);
    double& value = image->_get_value_unchecked(1, 1);
    value = -1;
    CHECK_EQ(image->get_value_unchecked(1, 1), -1);
}

TEST_CASE("VectorImageArray") {
    size_t n_rows = 3, n_cols = 2;
    std::shared_ptr<Image> image = std::make_shared<Image>(n_rows, n_cols);
    ImageArray::Data data = {image, image};
    ImageArray arr = ImageArray(&data);

    for (ImageArray::const_iterator it = arr.cbegin(); it != arr.cend(); ++it) {
        CHECK_EQ(*it, image);
    }
    for (const auto& img : arr) {
        CHECK_EQ(img, image);
    }
}

TEST_CASE("VectorMask") {
    auto image = Image(2, 2);
    auto mask = Mask(2, 2);
    CHECK_EQ(mask.get_value_unchecked(0, 0), false);
    mask._get_value_unchecked(1, 1) = true;
    CHECK_EQ(mask.get_value_unchecked(1, 1), true);
    CHECK_EQ(g2::images_compatible<double, Image, bool, Mask>(image, mask), true);
}
