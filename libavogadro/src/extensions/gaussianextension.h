/**********************************************************************
  GaussianExtension - Extension for generating Gaussian input decks

  Copyright (C) 2008 Marcus D. Hanwell

  This file is part of the Avogadro molecular editor project.
  For more information, see <http://avogadro.openmolecules.net/>

  Avogadro is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Avogadro is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
 **********************************************************************/

#ifndef GAUSSIANEXTENSION_H
#define GAUSSIANEXTENSION_H

#include <avogadro/glwidget.h>
#include <avogadro/extension.h>

#include "gaussianinputdialog.h"
#include "qcheminputdialog.h"
#include "molproinputdialog.h"
#include "nwcheminputdialog.h"
#include "mopacinputdialog.h"

namespace Avogadro
{
  class GaussianExtension : public Extension
  {
  Q_OBJECT
    AVOGADRO_EXTENSION("Gaussian Input Deck", tr("Gaussian Input Deck"), 
                       tr("Create input files for quantum chemistry packages like Gaussian"))
  public:
    GaussianExtension(QObject* parent = 0);
    virtual ~GaussianExtension();

    virtual QList<QAction *> actions() const;

    virtual QString menuPath(QAction* action) const;

    virtual QUndoCommand* performAction(QAction *action, GLWidget *widget);

    void setMolecule(Molecule *molecule);

    /**
     * Save the settings for this extension.
     * @param settings Settings variable to write settings to.
     */
    void writeSettings(QSettings &settings) const;

    /**
     * Read the settings for this extension.
     * @param settings Settings variable to read settings from.
     */
    void readSettings(QSettings &settings);
    
  public Q_SLOTS:
    void readOutputFile(const QString filename);

  private:
    GaussianInputDialog* m_gaussianInputDialog;
    QChemInputDialog* m_qchemInputDialog;
    MOPACInputDialog* m_mopacInputDialog;
    NWChemInputDialog* m_nwchemInputDialog;
    MolproInputDialog* m_molproInputDialog;
    QList<QAction *> m_actions;
    Molecule *m_molecule;
    
    GLWidget *m_widget;

  };

  class GaussianExtensionFactory : public QObject, public PluginFactory
  {
    Q_OBJECT
    Q_INTERFACES(Avogadro::PluginFactory)
    AVOGADRO_EXTENSION_FACTORY(GaussianExtension) 
  };

} // End namespace Avogadro

#endif
