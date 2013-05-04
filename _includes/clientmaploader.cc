//------------------------------------------------------------------------------
//  clientmaploader.cc
//  (C) 2010 Bigpoint GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "clientmaploader.h"
#include "io/ioserver.h"
#include "maploader.h"
#include "mapdata.h"

namespace Map
{
using namespace IO;
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
bool
ClientMapLoader::Load(const StringAtom& name, const URI& uri, const Ptr<ClientMap>& outMap)
{
    bool result = false;
    this->mapName = name;
    this->stringTable.Clear();
    this->error.Clear();

    Ptr<MapData> mapData = MapData::Create();
    MapLoader mapLoader;
    if (mapLoader.LoadMap(uri, mapData))
    {
        // map infos
        bbox mapBox(mapData->info.center, mapData->info.extents);
        outMap->Setup(this->mapName, mapData->info.gridSize, mapBox, 
                      mapData->info.mapSizeX, mapData->info.mapSizeY, mapData->info.mapSizeZ);

        // string table
        this->stringTable = mapData->stringTable;

        // templates
        outMap->ReserveTemplates(mapData->templates.Size());
        IndexT templIndex;
        SizeT numTemplates = mapData->templates.Size();
        for (templIndex = 0; templIndex < numTemplates; templIndex++)
        {
            const MapData::Template& curTemplate = mapData->templates[templIndex];
            ClientMapItemTemplate::Type type = (ClientMapItemTemplate::Type) curTemplate.type;

            bbox box(curTemplate.center, curTemplate.extents);
            if (type == ClientMapItemTemplate::IndependentCollide)
            {
                outMap->AddTemplate(this->LookupString(curTemplate.collResId),
                                    curTemplate.collMeshGroupIndex, box, type);
            }
            else if (type != ClientMapItemTemplate::DestructibleActive &&
                     type != ClientMapItemTemplate::DestructibleInactive &&
                     type != ClientMapItemTemplate::DestructibleGoingInactive)
            {
                outMap->AddTemplate(this->LookupString(curTemplate.gfxResId),
                                    this->LookupString(curTemplate.collResId),
                                    curTemplate.collMeshGroupIndex, box, type);
            }
            else
            {
                outMap->AddTemplate(this->LookupString(curTemplate.gfxResId),
                                    this->LookupString(curTemplate.gfxRootNode),
                                    this->LookupString(curTemplate.phxResId),
                                    this->LookupString(curTemplate.collResId),
                                    curTemplate.collMeshGroupIndex, box, type,
                                    this->LookupString(curTemplate.sfxEventId));
            }
        }

        // event list
        Dictionary<StringAtom, uchar> eventIds;
        eventIds.BeginBulkAdd();
        IndexT eventIndex;
        SizeT numEvents = mapData->eventList.Size();
        for (eventIndex = 0; eventIndex < numEvents; eventIndex++)
        {
            StringAtom eventName = this->LookupString(mapData->eventList[eventIndex]);
            this->eventList.Append(eventName);
            eventIds.Add(eventName, 0);
        }
        eventIds.EndBulkAdd();
        outMap->SetPossibleEventIds(eventIds);

        // event mapping table
        this->eventMappingTable = mapData->eventMapping;

        // instances
        IndexT instIndex;
        SizeT numInstances = mapData->instances.Size();
        Dictionary<StringAtom, bool> eventListInstance;
        const float4 nullPos(0.0f, 0.0f, 0.0f, 1.0f);
        const StringAtom dummyEditName = "";
        for (instIndex = 0; instIndex < numInstances; instIndex++)
        {
            eventListInstance.Clear();
            const MapData::Instance& curInstance = mapData->instances[instIndex];
            
            // FIXME FIXME FIXME: WTF? this is way to expensive!
            // load a default instance, which may be switch of in some events
            if (curInstance.indexToMapping != 0)
            {
                n_assert(curInstance.indexToMapping < this->eventMappingTable.Size());
                const String& mapping = this->eventMappingTable[curInstance.indexToMapping];
                int numEvents = this->eventList.Size();
                int eventIdx;
                for (eventIdx = 0; eventIdx < numEvents; ++eventIdx)
                {
                    if (mapping[eventIdx] == '0')
                    {
                        eventListInstance.Add(this->eventList[eventIdx], false);
                    }
                }
            }

            matrix44 transform = matrix44::affinetransformation(curInstance.scale, nullPos, curInstance.rot, curInstance.pos);
            bbox box = outMap->TemplateByIndex(curInstance.templIndex).GetBoundingBox();
            box.transform(transform);
            outMap->AddInstance(transform, box, curInstance.templIndex,
                curInstance.groupIndex, dummyEditName,
                curInstance.useCollide, eventListInstance);
        }

        // groups (FIXME: actually used?)
        Array<SizeT> instanceIndices;
        Array<SizeT> groupIndices;
        IndexT groupIndex;
        SizeT numGroups = mapData->groups.Size();
        for (groupIndex = 0; groupIndex < numGroups; groupIndex++)
        {
            const MapData::Group& curGroup = mapData->groups[groupIndex];
            StringAtom groupName = this->LookupString(curGroup.nameId);
            ClientMapGroup::Type groupType = (ClientMapGroup::Type) curGroup.type;

            IndexT instIndexIndex;
            SizeT numInstanceIndices = curGroup.instanceIndices.Size();
            instanceIndices.Clear();
            instanceIndices.Reserve(numInstanceIndices);
            for (instIndexIndex = 0; instIndexIndex < numInstanceIndices; instIndexIndex++)
            {
                instanceIndices.Append(curGroup.instanceIndices[instIndexIndex]);
            }

            IndexT groupIndexIndex;
            SizeT numGroupIndices = curGroup.groupIndices.Size();
            groupIndices.Clear();
            groupIndices.Reserve(numGroupIndices);
            for (groupIndexIndex = 0; groupIndexIndex < numGroupIndices; groupIndexIndex++)
            {
                groupIndices.Append(curGroup.groupIndices[groupIndexIndex]);
            }

            outMap->AddGroup(groupName, groupType, curGroup.parentGroupIndex,
                             instanceIndices, groupIndices);
        }

        result = true;
    }
    else
    {
        this->error = "Failed to open map file!";
        result = false;
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
bool
ClientMapLoader::LoadEventContent(const StringAtom& eventID, const URI& uri, const Ptr<ClientMap>& parentMap)
{
    bool result = false;

    if (parentMap->HasEventMap(eventID))
    {
        Array<MapData::Instance> mapDataInstances;
        Array<MapData::Group> mapDataGroups;
        MapLoader mapLoader;
        if (mapLoader.LoadAdditionalEvents(uri, mapDataInstances, mapDataGroups))
        {
            int parentMapInstanceOffset = parentMap->GetNumInstances();
            
            // instances
            IndexT instIndex;
            SizeT numInstances = mapDataInstances.Size();
            Dictionary<StringAtom, bool> eventListInstance;
            const float4 nullPos(0.0f, 0.0f, 0.0f, 1.0f);
            const StringAtom dummyEditName = "";            
            for (instIndex = 0; instIndex < numInstances; instIndex++)
            {
                const MapData::Instance& curInstance = mapDataInstances[instIndex];
                eventListInstance.Clear();
                
                // FIXME: WTF?
                if (eventID.IsValid())
                {
                    // load a event only instance
                    eventListInstance.Add(eventID, true);
                }
                matrix44 transform = matrix44::affinetransformation(curInstance.scale,
                    nullPos, curInstance.rot, curInstance.pos);

                // compute the instance bounding box
                bbox box = parentMap->TemplateByIndex(curInstance.templIndex).GetBoundingBox();
                box.transform(transform);

                parentMap->AddInstance(transform, box, curInstance.templIndex,
                    curInstance.groupIndex, dummyEditName, curInstance.useCollide, eventListInstance);
            }

            n_assert2(parentMap->GetNumGroups() == mapDataGroups.Size(), "Number of groups in map file does not match event file");
            
            // groups
            IndexT groupIndex;
            SizeT numGroups = mapDataGroups.Size();
            for (groupIndex = 0; groupIndex < numGroups; groupIndex++)
            {
                const MapData::Group& curGroup = mapDataGroups[groupIndex];
                int numInstIndices = curGroup.instanceIndices.Size();
                if (numInstIndices > 0)
                {
                    Array<IndexT> newIndices;
                    newIndices.Reserve(numInstIndices);
                    IndexT instIndexIndex;
                    for (instIndexIndex = 0; instIndexIndex < numInstIndices; instIndexIndex++)
                    {
                        newIndices.Append(curGroup.instanceIndices[instIndexIndex] + parentMapInstanceOffset);
                    }
                    parentMap->UpdateGroup(groupIndex, newIndices);
                }
            }
            result = true;
        }
        else
        {
            this->error = "Failed to open additional map file!";
            result = false;
        }
    }
    return result;
}

//------------------------------------------------------------------------------
/**
*/
StringAtom
ClientMapLoader::LookupString(ushort stringIndex)
{
    if (InvalidStringIndex != stringIndex)
    {
        return this->stringTable[stringIndex];
    }
    else
    {
        StringAtom invalidStringAtom;
        return invalidStringAtom;
    }
}

} // namespace Map
