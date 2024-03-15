## ItemRPG

A Unreal Engine 5 MMO trying to copycat PalWorld using 'C++'

## 🛠️ Tooling

#### Tilesets


```bash
여기에 내용 추가해보기
```

## ✅ Todo list

#### Items

- [ ] Suffix / Prefix system improvements
- [ ] Spawned item mods `min` needs to take `max` into account
- [ ] Gloves items

#### Systems 

- [ ] 거래 창
- [ ] 미니맵
- [ ] Doors that require a key or level to pass through
- [ ] Map item drops (Mining, Herbs) Professions?
- [ ] Ability to choose from mage-type rogue-type or warrior-type rewards on some quests

#### Combat

- [ ] Negative buffs: (Frozen, Stunned, etc.)
- [ ] Allow critical strikes on spells
- [ ] Ranged weapons 🏹
- [ ] Serverside individual cooldowns for spells
- [ ] More spells that effect movement, (dives, dodges, pushback etc)
- [ ] Back-stab damage
- [ ] Show warning before enemies attack

#### Performance

- [ ] Predictive hero movement so we do not have to send `[x,y]` @ 60fps and so other players aren't able to hit you when you are not near them.
- [ ] Multiple `Realms` because 1000 people in a map will not scale.
- [ ] How to make servers smart about not loading map rooms until a person is actually in them.
- [ ] Move spell cooldowns to serverside check

#### Completed

- [x] Readable signs objects
- [x] Hold to aim spells
- [x] Helmets that can hide hair or face
- [x] Elemental Damage / Resistances
- [x] Inventory Bags system
- [x] Party system
- [x] Enemy spells
- [x] Buffs
- [x] Facial hair is now Whiskers on its own layer seperate from face.
- [x] Consumables and Food
- [x] Cooldowns show in UI
- [x] Attack stamina
- [x] Fix party system to update stats properly
