// ASEPRITE gui library
// Copyright (C) 2001-2012  David Capello
//
// This source file is distributed under a BSD-like license, please
// read LICENSE.txt for more information.

#include "config.h"

#include "base/shared_ptr.h"
#include "ui/component.h"
#include "ui/property.h"

namespace ui {

Component::Component()
{
}

Component::~Component()
{
}

PropertyPtr Component::getProperty(const base::string& name)
{
  Properties::iterator it = m_properties.find(name);
  if (it != m_properties.end())
    return it->second;
  else
    return PropertyPtr();
}

void Component::setProperty(PropertyPtr property)
{
  m_properties[property->getName()] = property;
}

bool Component::hasProperty(const base::string& name)
{
  Properties::iterator it = m_properties.find(name);
  return it != m_properties.end();
}

void Component::removeProperty(const base::string& name)
{
  Properties::iterator it = m_properties.find(name);
  if (it != m_properties.end())
    m_properties.erase(it);
}

const Component::Properties& Component::getProperties() const
{
  return m_properties;
}

} // namespace ui
