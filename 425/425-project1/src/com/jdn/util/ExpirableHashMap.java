package com.jdn.util;

import static java.util.Collections.unmodifiableSet;
import static java.util.Collections.unmodifiableCollection;

import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;


public class ExpirableHashMap<K, V> implements Map<K, V> {

    private final long ttlMs;
    private final Map<K, V> map = new HashMap<K, V>();
    private final Map<K, Long> creationTimes = new HashMap<K, Long>();

    public ExpirableHashMap(long ttlMs) {
        this.ttlMs = ttlMs;
    }

    @Override
    public int size() {
        return map.size();
    }

    @Override
    public boolean isEmpty() {
        return map.isEmpty();
    }

    @Override
    public boolean containsKey(Object key) {
        return map.containsKey(key);
    }

    @Override
    public boolean containsValue(Object value) {
        return map.containsValue(value);
    }

    @Override
    public V get(Object key) {
        V result = map.get(key);
        if (result != null && !isExpired(key)) {
            map.remove(key);
            creationTimes.remove(key);
            return result;
        }
        return null;
    }

    @Override
    public V put(K key, V value) {
        creationTimes.put(key, System.currentTimeMillis());
        return map.put(key, value);
    }

    @Override
    public V remove(Object key) {
        creationTimes.remove(key);
        return map.remove(key);
    }

    @Override
    public void putAll(Map<? extends K, ? extends V> map) {
        for (Entry<? extends K, ? extends V> entry : map.entrySet()) {
            put(entry.getKey(), entry.getValue());
        }
    }

    @Override
    public void clear() {
        creationTimes.clear();
        map.clear();
    }

    @Override
    public Set<K> keySet() {
        expireRecords();
        return unmodifiableSet(map.keySet());
    }

    @Override
    public Collection<V> values() {
        expireRecords();
        return unmodifiableCollection(map.values());
    }

    @Override
    public Set<Entry<K, V>> entrySet() {
        expireRecords();
        return unmodifiableSet(map.entrySet());
    }

    /**
     * Whether or not the value associated with the input key is expired.
     *
     * @param key The key to check
     * @return expired True if expired, false if not expired or does not exist
     */
    public boolean isExpired(Object key) {
        boolean expired = false;
        Long creationTime = creationTimes.get(key);
        if (creationTime != null) {
            expired = System.currentTimeMillis() - creationTime > ttlMs;
        }
        return expired;
    }

    /**
     * Remove all expired records.
     */
    public void expireRecords() {
        for (K key : map.keySet()) {
            if (isExpired(key)) {
                remove(key);
            }
        }
    }

}
