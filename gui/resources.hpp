/*
	Copyright 2015 TeamWin
	This file is part of TWRP/TeamWin Recovery Project.

	TWRP is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	TWRP is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with TWRP.  If not, see <http://www.gnu.org/licenses/>.
*/

// resources.hpp - Base classes for resource management of GUI

#ifndef _RESOURCES_HPP_HEADER
#define _RESOURCES_HPP_HEADER

#include <map>
#include <stddef.h>
#include <string>
#include <vector>

#include "../minuitwrp/minui.h"
#include "rapidxml.hpp"

using namespace rapidxml;

struct ZipArchive;

// Base Objects
class Resource
{
public:
	Resource(xml_node<>* node, ZipArchive* pZip);
	virtual ~Resource() {}

public:
	std::string GetName() { return mName; }

private:
	std::string mName;

protected:
	static int ExtractResource(ZipArchive* pZip, std::string folderName, std::string fileName, std::string fileExtn, std::string destFile);
	static void LoadImage(ZipArchive* pZip, std::string file, gr_surface* source);
	static void CheckAndScaleImage(gr_surface source, gr_surface* destination, int retain_aspect);
};

class FontResource : public Resource
{
public:
	enum Type
	{
		TYPE_TWRP,
#ifndef TW_DISABLE_TTF
		TYPE_TTF,
#endif
	};

	FontResource(xml_node<>* node, ZipArchive* pZip);
	virtual ~FontResource();

public:
	void* GetResource() { return this ? mFont : NULL; }
	int GetHeight() { return gr_getMaxFontHeight(this ? mFont : NULL); }
	void Override(xml_node<>* node, ZipArchive* pZip);

protected:
	void* mFont;
	Type m_type;

private:
	void LoadFont(xml_node<>* node, ZipArchive* pZip);
	void DeleteFont();

private:
	int origFontSize;
	void* origFont;
};

class ImageResource : public Resource
{
public:
	ImageResource(xml_node<>* node, ZipArchive* pZip);
	virtual ~ImageResource();

public:
	gr_surface GetResource() { return this ? mSurface : NULL; }
	int GetWidth() { return gr_get_width(this ? mSurface : NULL); }
	int GetHeight() { return gr_get_height(this ? mSurface : NULL); }

protected:
	gr_surface mSurface;
};

class AnimationResource : public Resource
{
public:
	AnimationResource(xml_node<>* node, ZipArchive* pZip);
	virtual ~AnimationResource();

public:
	gr_surface GetResource() { return (!this || mSurfaces.empty()) ? NULL : mSurfaces.at(0); }
	gr_surface GetResource(int entry) { return (!this || mSurfaces.empty()) ? NULL : mSurfaces.at(entry); }
	int GetWidth() { return gr_get_width(this ? GetResource() : NULL); }
	int GetHeight() { return gr_get_height(this ? GetResource() : NULL); }
	int GetResourceCount() { return mSurfaces.size(); }

protected:
	std::vector<gr_surface> mSurfaces;
};

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	void AddStringResource(std::string resource_source, std::string resource_name, std::string value);
	void LoadResources(xml_node<>* resList, ZipArchive* pZip, std::string resource_source);

public:
	FontResource* FindFont(const std::string& name) const;
	ImageResource* FindImage(const std::string& name) const;
	AnimationResource* FindAnimation(const std::string& name) const;
	std::string FindString(const std::string& name) const;
	std::string FindString(const std::string& name, const std::string& default_string) const;
	void DumpStrings() const;

private:
	struct string_resource_struct {
		std::string value;
		std::string source;
	};
	std::vector<FontResource*> mFonts;
	std::vector<ImageResource*> mImages;
	std::vector<AnimationResource*> mAnimations;
	std::map<std::string, string_resource_struct> mStrings;
};

#endif  // _RESOURCES_HPP_HEADER
