/*
 * Copyright 2011 QZXing authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CAMERAIMAGE_H
#define CAMERAIMAGE_H

#include <QImage>
#include <QString>
#include <zxing/zxing/common/GreyscaleLuminanceSource.h>

using namespace zxing;

class CameraImageWrapper : public LuminanceSource
{
public:
    CameraImageWrapper();
    CameraImageWrapper(const QImage& sourceImage);
    CameraImageWrapper(CameraImageWrapper& otherInstance);
    ~CameraImageWrapper();

    static QSharedPointer<CameraImageWrapper> Factory(const QImage& image, int maxWidth=-1, int maxHeight=-1, bool smoothTransformation=false);
    
    QSharedPointer<std::vector<QSharedPointer<std::vector<zxing::byte>> > > getOriginalImage();
    QSharedPointer<GreyscaleLuminanceSource> getDelegate() { return delegate; }

    QSharedPointer<std::vector<zxing::byte>> getRow(int y, QSharedPointer<std::vector<zxing::byte>> row) const;
    QSharedPointer<std::vector<zxing::byte>> getMatrix() const;

    bool isCropSupported() const;
    QSharedPointer<LuminanceSource> crop(int left, int top, int width, int height) const;
    bool isRotateSupported() const;
    QSharedPointer<LuminanceSource> invert() const;
    QSharedPointer<LuminanceSource> rotateCounterClockwise() const;

    inline zxing::byte gray(const unsigned int r, const unsigned int g, const unsigned int b);
  
private:
    QSharedPointer<std::vector<zxing::byte>> getRowP(int y, QSharedPointer<std::vector<zxing::byte>> row) const;
    QSharedPointer<std::vector<zxing::byte>> getMatrixP() const;
    void updateImageAsGrayscale(const QImage &origin);

    QSharedPointer<GreyscaleLuminanceSource> delegate;
    QSharedPointer<std::vector<QSharedPointer<std::vector<zxing::byte>>>> imageBytesPerRow;
    QSharedPointer<std::vector<zxing::byte>> imageBytes;

    static const zxing::byte B_TO_GREYSCALE[256];
    static const zxing::byte G_TO_GREYSCALE[256];
    static const zxing::byte R_TO_GREYSCALE[256];
};

#endif //CAMERAIMAGE_H
