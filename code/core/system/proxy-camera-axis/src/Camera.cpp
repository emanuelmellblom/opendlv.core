/**
 * proxy-camera-axis - Interface to network cameras from Axis.
 * Copyright (C) 2012 - 2015 Christian Berger
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <iostream>

#include <opendavinci/odcore/base/Lock.h>
#include <opendavinci/odcore/wrapper/SharedMemoryFactory.h>

#include "Camera.h"

namespace opendlv {
namespace core {
namespace system {
namespace proxy {

using namespace odcore::base;

Camera::Camera(const string &name, const uint32_t &width, const uint32_t &height)
    : m_sharedImage()
    , m_sharedMemory()
    , m_name(name)
    , m_width(width)
    , m_height(height)
    , m_size(0) {
    const uint32_t BPP = 3;
    m_size = width * height * BPP;

    m_sharedMemory = odcore::wrapper::SharedMemoryFactory::createSharedMemory(name, m_size);

    m_sharedImage.setName(name);
    m_sharedImage.setSize(m_size);
    m_sharedImage.setWidth(width);
    m_sharedImage.setHeight(height);
    m_sharedImage.setBytesPerPixel(BPP);
}

Camera::~Camera() {}

const string Camera::getName() const {
    return m_name;
}

uint32_t Camera::getWidth() const {
    return m_width;
}

uint32_t Camera::getHeight() const {
    return m_height;
}

uint32_t Camera::getSize() const {
    return m_size;
}

odcore::data::image::SharedImage Camera::capture() {
    if (isValid()) {
        if (captureFrame()) {
            if (m_sharedMemory.get() && m_sharedMemory->isValid()) {
                Lock l(m_sharedMemory);
                copyImageTo(static_cast<char*>(m_sharedMemory->getSharedMemory()), m_size);
            }
        }
    }

    return m_sharedImage;
}
}
}
}
} // opendlv::core::system::proxy
