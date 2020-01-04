#include "stdafx.h"
#include "SongFilter.hpp"

Map<int32, SongSelectIndex> LevelFilter::GetFiltered(const Map<int32, SongSelectIndex>& source)
{
	Map<int32, SongSelectIndex> filtered;
	for (auto kvp : source)
	{
		for (auto diff : kvp.second.GetDifficulties())
		{
			if (diff->settings.level == m_level)
			{
				SongSelectIndex index(kvp.second.GetMap(), diff);
				filtered.Add(index.id, index);
			}
		}
	}
	return filtered;
}

String LevelFilter::GetName() const
{
	return Utility::Sprintf("Level: %d", m_level);
}

bool LevelFilter::IsAll() const
{
	return false;
}

Map<int32, SongSelectIndex> FolderFilter::GetFiltered(const Map<int32, SongSelectIndex>& source)
{
	Map<int32, MapIndex*> maps = m_mapDatabase->FindMapsByFolder(m_folder);

	Map<int32, SongSelectIndex> filtered;
	for (auto m : maps)
	{
		SongSelectIndex index(m.second);
		filtered.Add(index.id, index);
	}
	return filtered;
}

String FolderFilter::GetName() const
{
	return "Folder: " + m_folder;
}

bool FolderFilter::IsAll() const
{
	return false;
}

Map<int32, SongSelectIndex> CollectionFilter::GetFiltered(const Map<int32, SongSelectIndex>& source)
{
	Map<int32, MapIndex*> maps = m_mapDatabase->FindMapsByCollection(m_collection);

	Map<int32, SongSelectIndex> filtered;
	for (auto m : maps)
	{
		SongSelectIndex index(m.second);
		filtered.Add(index.id, index);
	}
	return filtered;
}

String CollectionFilter::GetName() const
{
	return "Collection: " + m_collection;
}

bool CollectionFilter::IsAll() const
{
	return false;
}
