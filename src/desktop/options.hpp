
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_OPTIONS_HPP_INCLUDED
#define STOCK_OPTIONS_HPP_INCLUDED

// Qt include.
#include <QDialog>

// C++ include.
#include <memory>


namespace Stock {

//
// Options
//

class OptionsPrivate;

//! Options.
class Options Q_DECL_FINAL
	:	public QDialog
{
	Q_OBJECT

public:
	Options( const QString & host, quint16 port, const QString & pwd, QWidget * parent );
	virtual ~Options();

	//! \return Host.
	QString host() const;
	//! \return Port.
	quint16 port() const;
	//! \return Password.
	QString password() const;

private slots:
	//! Show/hide password.
	void showHidePassword();
	//! Host or password changed.
	void hostPasswordChanged( const QString & );

private:
	friend class OptionsPrivate;

	Q_DISABLE_COPY( Options )

	std::unique_ptr< OptionsPrivate > d;
}; // class Options

} /* namespace Stock */

#endif // STOCK_OPTIONS_HPP_INCLUDED
