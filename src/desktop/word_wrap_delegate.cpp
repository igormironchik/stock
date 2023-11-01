
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

// Globe include.
#include "word_wrap_delegate.hpp"

// Qt include.
#include <QHeaderView>
#include <QPainter>


namespace Stock {

//
// WordWrapItemDelegate
//

WordWrapItemDelegate::WordWrapItemDelegate( QTreeView * parent )
	:	QStyledItemDelegate( parent )
	,	m_parent( parent )
{
}

QSize
WordWrapItemDelegate::sizeHint( const QStyleOptionViewItem & option,
	const QModelIndex & index ) const
{
	return option.fontMetrics.boundingRect(
		QRect( 0, 0, m_parent->header()->sectionSize( index.column() ), 0 ),
		Qt::AlignLeft | Qt::TextWordWrap,
		index.data( Qt::DisplayRole ).toString() ).size();
}

void
WordWrapItemDelegate::paint( QPainter * painter,
	const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QStyledItemDelegate::paint( painter, option, QModelIndex() );

	painter->drawText( option.rect,
		Qt::AlignLeft | Qt::TextWordWrap | Qt::AlignVCenter,
		index.data( Qt::DisplayRole ).toString() );
}

} /* namespace Stock */
