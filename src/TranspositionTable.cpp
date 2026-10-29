#include "TranspositionTable.h"

TranspositionTable::TranspositionTable(int megabytes)
{
    uint64 required_size = megabytes * 1024 * 1024 / sizeof(TranspositionEntry);
    table_size = 1;
    while (table_size <= required_size)
        table_size *= 2;
    table = new TranspositionEntry[table_size]();
}

TranspositionEntry *TranspositionTable::probe(Hash hash)
{
    Hash index = modPow2(hash, table_size);
    TranspositionEntry &entry = table[index];
    if (entry.hash != hash)
    {
        if (entry.hash != 0)
            hash_probe_collisions++;
        hash_probe_misses++;
        return nullptr;
    }
    hash_probe_hits++;
    return &entry;
}

void TranspositionTable::store(Hash hash, Score score, Move best_move, int remaining_depth, BoundType type)
{
    Hash index = modPow2(hash, table_size);
    TranspositionEntry &entry = table[index];

    hash_store_attempts++;

    if (entry.hash != 0 && entry.hash != hash)
        hash_store_collisions++;

    // do not store if present entry has greater depth
    if (entry.remaining_depth > remaining_depth)
        return;

    // do not store if present entry is exact, while new is not exact, keep present entry
    if (entry.hash == hash && (entry.type == EXACT && type != EXACT))
        return;

    hash_store_updates++;
    entry.hash = hash;
    entry.score = score;
    entry.best_move = best_move;
    entry.remaining_depth = remaining_depth;
    entry.type = type;
}

void TranspositionTable::clear()
{
    hash_probe_hits = 0;
    hash_probe_misses = 0;
    hash_probe_collisions = 0;
    hash_store_attempts = 0;
    hash_store_updates = 0;
    hash_store_collisions = 0;
    for (uint64 i = 0; i < table_size; i++)
    {
        table[i].hash = 0;
        table[i].score = 0;
        table[i].best_move = NULL_MOVE;
        table[i].remaining_depth = 0;
        table[i].type = EXACT;
    }
}

void TranspositionTable::printStatistics()
{
    log(TRANSPOSITION_TABLE, "Transposition Table Statistics:");
    uint64 hash_probes = hash_probe_hits + hash_probe_misses;
    std::string hit_rate = getFormattedPercentage(hash_probe_hits, hash_probes);
    log(TRANSPOSITION_TABLE, "\tProbe Hits: " + getFormattedInteger(hash_probe_hits) + ", Misses: " + getFormattedInteger(hash_probe_misses) + ", Accesses: " + getFormattedInteger(hash_probes) + ", Hit Rate: " + hit_rate);
    std::string store_scuccess_rate = getFormattedPercentage(hash_store_updates, hash_store_attempts);
    log(TRANSPOSITION_TABLE, "\tStore Attempts: " + getFormattedInteger(hash_store_attempts) + ", Updates: " + getFormattedInteger(hash_store_updates) + ", Success Rate: " + store_scuccess_rate);
    std::string store_collision_rate = getFormattedPercentage(hash_store_collisions, hash_store_attempts);
    log(TRANSPOSITION_TABLE, "\tHash Store Collisions: " + getFormattedInteger(hash_store_collisions) + ", Store Collision Rate: " + store_collision_rate);
    std::string probe_collision_rate = getFormattedPercentage(hash_probe_collisions, hash_probes);
    log(TRANSPOSITION_TABLE, "\tHash Probe Collisions: " + getFormattedInteger(hash_probe_collisions) + ", Probe Collision Rate: " + probe_collision_rate);
}
