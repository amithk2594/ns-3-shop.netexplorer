/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Andrey Churin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Andrey Churin <aachurin@gmail.com>
 */

#ifndef NET_MODEL_H
#define NET_MODEL_H

#include <gtkmm.h>
#include <istream>

#define ENSURE_REGISTER_MODEL(type)                       \
  static struct _MODEL_##type##_RegistrationClass         \
  {                                                       \
    _MODEL_##type##_RegistrationClass () {                \
        ModelFactory::RegisterModel (                     \
          type::GetModelTypeId ()                         \
        );                                                \
    }                                                     \
  } _MODEL_##type##_RegistrationVariable

class NetModel;

class ModelTypeId
{
public:
  ModelTypeId (const Glib::ustring &name, const sigc::slot<NetModel*> &factory = sigc::slot<NetModel*> (),
    const Glib::ustring &descr = Glib::ustring (), const Glib::ustring &mimeType = Glib::ustring (),
    const Glib::ustring &pattern = Glib::ustring (), bool readable = false, bool writable = false);

  Glib::ustring GetName (void) const;
  NetModel* Create (void) const;
  Glib::ustring GetDescription (void) const;
  Glib::ustring GetMimeType (void) const;
  Glib::ustring GetPattern (void) const;
  bool IsReadable (void) const;
  bool IsWritable (void) const;
  operator Glib::ustring (void) const;
  bool operator== (const ModelTypeId &typeId) const;
  bool operator!= (const ModelTypeId &typeId) const;

private:
  Glib::ustring m_name;
  sigc::slot<NetModel*> m_factory;
  Glib::ustring m_description;
  Glib::ustring m_mimeType;
  Glib::ustring m_pattern;
  bool m_readable;
  bool m_writable;
};

class ModelFactory
{
public:
  typedef std::map<Glib::ustring, ModelTypeId>::const_iterator Iterator;
  static void RegisterModel (const ModelTypeId &id);
  static NetModel* Create (const Glib::ustring &name);
  static ModelTypeId GetModel (const Glib::ustring &name);
  static Iterator Begin (void);
  static Iterator End (void);
private:
  static std::map<Glib::ustring, ModelTypeId>* GetModelMap (void);
};

/**
 * \ingroup netexplorer
 * Model exception
 */
class ModelError: public Glib::Exception
{
public:
  ModelError (const Glib::ustring &message);
  virtual ~ModelError() throw();
  virtual Glib::ustring what() const;

private:
  Glib::ustring m_message;
};

/**
 * \ingroup netexplorer
 * Network View Model
 */
class NetModel: public Gtk::VBox
{
public:
  NetModel ();
  NetModel (const Glib::ustring &ui);
  virtual ~NetModel ();

  static ModelTypeId GetModelTypeId (void);
  /**
   * \brief Initialize view
   */
  virtual void Initialize (void) = 0;
  /**
   * \brief Reset model
   */
  virtual void Reset (void);
  /**
   * \returns true if no errors
   * Read model from stream
   */
  virtual bool ReadFromStream (Glib::RefPtr<Gio::DataInputStream> stream) = 0;
  /**
   * \returns true if no errors
   * Write model to stream
   */
  virtual bool WriteToStream (Glib::RefPtr<Gio::DataOutputStream> stream) = 0;
  /**
   * \param path a path
   * Looks up an action by following a path.
   */
  virtual Glib::RefPtr<Gtk::Action> GetAction (const Glib::ustring& path) const;
  /**
   * \return Accel group
   */
  virtual Glib::RefPtr<Gtk::AccelGroup> GetAccelGroup (void) const;
  /**
   * \return Action group
   */
  virtual Glib::RefPtr<Gtk::ActionGroup> GetActionGroup (void) const;

protected:
  template<typename T> T* GetUI (const char *ui) const;
  virtual void LoadUI (const Glib::ustring &ui);

private:
  void InitializeModel (void);

  Glib::RefPtr<Gtk::UIManager> m_uiManager;
};

template<typename T>
T*
NetModel::GetUI (const char *ui) const
{
  return (T*)m_uiManager->get_widget (ui);
}

#endif /* NET_MODEL_H */
