
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_RENAME_HPP_INCLUDED
#define STOCK_RENAME_HPP_INCLUDED

// Qt include.
#include <QDialog>

// C++ include.
#include <memory>


namespace Stock {

//
// RenameDlg
//

class RenameDlgPrivate;

//! RenameDlg.
class RenameDlg Q_DECL_FINAL
	:	public QDialog
{
	Q_OBJECT

public:
	RenameDlg( const QString & oldName, const QStringList & constraint,
		const QString & varName, QWidget * parent );
	virtual ~RenameDlg();

	//! \return New name.
	QString renamed() const;

private slots:
	//! Name changed.
	void nameChanged( const QString & txt );

private:
	friend class RenameDlgPrivate;

	Q_DISABLE_COPY( RenameDlg )

	std::unique_ptr< RenameDlgPrivate > d;
}; // class RenameDlg

} /* namespace Stock */

#endif // STOCK_RENAME_HPP_INCLUDED
