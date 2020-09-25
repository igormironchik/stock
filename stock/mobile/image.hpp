
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2018-2020 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
