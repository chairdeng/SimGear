// Base class for property controlled subsystems
//
// Copyright (C) 2012  Thomas Geymayer <tomgey@gmail.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "PropertyBasedMgr.hxx"

#include <stdexcept>
#include <string>

namespace simgear
{

  //----------------------------------------------------------------------------
  void PropertyBasedMgr::init()
  {
    _props->addChangeListener(this);
    _props->fireCreatedRecursive();
  }

  //----------------------------------------------------------------------------
  void PropertyBasedMgr::shutdown()
  {
    _props->removeChangeListener(this);
  }

  //----------------------------------------------------------------------------
  void PropertyBasedMgr::update(double delta_time_sec)
  {
    for( size_t i = 0; i < _elements.size(); ++i )
      if( _elements[i] )
        _elements[i]->update(delta_time_sec);
  }

  //----------------------------------------------------------------------------
  void PropertyBasedMgr::childAdded( SGPropertyNode * parent,
                                     SGPropertyNode * child )
  {
    if( parent != _props || child->getNameString() != _name_elements )
      return;

    size_t index = child->getIndex();

    if( index >= _elements.size() )
    {
      if( index > _elements.size() )
        SG_LOG
        (
          SG_GENERAL,
          SG_WARN,
          "Skipping unused " << _name_elements << " slot(s)!"
        );

      _elements.resize(index + 1);
    }
    else if( _elements[index] )
      SG_LOG
      (
        SG_GENERAL,
        SG_WARN,
        _name_elements << "[" << index << "] already exists!"
      );

    PropertyBasedElementPtr el = _element_factory(child);
    el->_self = el;
    _elements[index] = el;
    elementCreated( el );
  }

  //----------------------------------------------------------------------------
  void PropertyBasedMgr::childRemoved( SGPropertyNode * parent,
                                       SGPropertyNode * child )
  {
    if( parent != _props )
      return child->fireChildrenRemovedRecursive();
    else if( child->getNameString() != _name_elements )
      return;

    size_t index = child->getIndex();

    if( index >= _elements.size() )
      SG_LOG
      (
        SG_GENERAL,
        SG_WARN,
        "can't removed unknown " << _name_elements << "[" << index << "]!"
      );
    else
      // remove the element...
      _elements[index].reset();
  }

  //----------------------------------------------------------------------------
  const SGPropertyNode* PropertyBasedMgr::getPropertyRoot() const
  {
    return _props;
  }

  //----------------------------------------------------------------------------
  PropertyBasedMgr::PropertyBasedMgr( SGPropertyNode_ptr props,
                                      const std::string& name_elements,
                                      ElementFactory element_factory ):
    _props( props ),
    _name_elements( name_elements ),
    _element_factory( element_factory )
  {

  }

  //----------------------------------------------------------------------------
  PropertyBasedMgr::~PropertyBasedMgr()
  {

  }

} // namespace simgear