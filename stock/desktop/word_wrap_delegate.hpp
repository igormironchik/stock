
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2018 Igor Mironchik

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

#ifndef STOCK_WORD_WRAP_DELEGATE_HPP_INCLUDED
#define STOCK_WORD_WRAP_DELEGATE_HPP_INCLUDED

// Qt include.
#include <QStyledItemDelegate>
#include <QTreeView>


namespace Stock {

//! Item delegate for word wrapping.
class WordWrapItemDelegate final
  :  public QStyledItemDelegate
{
public:
  WordWrapItemDelegate( QTreeView * parent = nullptr );

  QSize sizeHint( const QStyleOptionViewItem & option,
	const QModelIndex & index ) const override;
  void paint( QPainter * painter, const QStyleOptionViewItem & option,
	const QModelIndex & index) const override;


private:
  QTreeView * m_parent;
}; // class WordWrapItemDelegate

} /* namespace Stock */

#endif // STOCK_WORD_WRAP_DELEGATE_HPP_INCLUDED
