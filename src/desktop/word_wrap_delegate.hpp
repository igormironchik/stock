
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
