#include "compositors.hpp"

#include <OgreViewport.h>
#include <OgreCompositorManager.h>

using namespace MWRender;

Compositors::Compositors(Ogre::Viewport* vp) :
      mViewport(vp)
    , mEnabled(true)
{
}

Compositors::~Compositors()
{
    Ogre::CompositorManager::getSingleton().removeCompositorChain(mViewport);
}

void Compositors::setEnabled (const bool enabled)
{
    for (CompositorMap::iterator it=mCompositors.begin();
        it != mCompositors.end(); ++it)
    {
        Ogre::CompositorManager::getSingleton().setCompositorEnabled(mViewport, it->first, enabled && it->second.first);
    }
    mEnabled = enabled;
}

void Compositors::addCompositor (const std::string& name, const int priority)
{
    int id = 0;

    for (CompositorMap::iterator it=mCompositors.begin();
        it != mCompositors.end(); ++it)
    {
        if (it->second.second > priority)
            break;
        ++id;
    }
    Ogre::CompositorManager::getSingleton().addCompositor (mViewport, name, id);

    mCompositors[name] = std::make_pair(false, priority);
}

void Compositors::setCompositorEnabled (const std::string& name, const bool enabled)
{
    mCompositors[name].first = enabled;
    Ogre::CompositorManager::getSingleton().setCompositorEnabled (mViewport, name, enabled && mEnabled);
}
