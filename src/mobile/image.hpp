
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_IMAGE_HPP_INCLUDED
#define STOCK_IMAGE_HPP_INCLUDED

// Qt include.
#include <QQuickPaintedItem>
#include <QImage>


namespace Stock {

//
// PreviewImage
//

//! Preview image.
class PreviewImage
	:	public QQuickPaintedItem
{
	Q_OBJECT

	Q_PROPERTY( QImage image READ image WRITE setImage )

signals:
	//! Code detected.
	void codeDetected( const QString & code );

public:
	static void registerQmlType();

	explicit PreviewImage( QQuickItem * parent = nullptr );
	~PreviewImage() override;

	//! \return Image.
	QImage image() const;
	//! Set image.
	void setImage( const QImage & img );

	void paint( QPainter * painter ) override;

private slots:
	//! Width changed.
	void markDirty();
	//! Transform changed.
	void transformChanged();

private:
	Q_DISABLE_COPY( PreviewImage )

	//! Current frame.
	QImage m_image;
	//! Dirty?
	bool m_dirty;
	//! Thumbnail.
	QImage m_thumbnail;
	//! Transform.
	QTransform m_transform;
}; // class PreviewImage

} /* namespace Stock */

#endif // STOCK_IMAGE_HPP_INCLUDED
