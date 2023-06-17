<div style=";background-color:rgb(255, 174, 201);background-image:url(https://www.minebbs.com/attachments/background_pink2-png.46702/?hash=e7169daa82dc5c36fa3346f5ebf6f705);color:rgb(0,0,0)">

<div style="margin-right:20px;margin-left:20px;margin-top:10px;">

<h3>&gt; Owo</h3>



<div style="font-size:18px">

<p>&#8195;&#8195;首先强烈安利 <a href="https://github.com/Amulet-Team/Amulet-Map-Editor/releases" target="_blank" title="逛完那边别忘了回来哦owo">Amulet</a>，这是一款绝赞的基岩版地图编辑器！提供了诸如任意角度旋转、删除区块、存档转换和结构导出等大量实用功能。有了它，对服务器地图的维护会变得非常简单！</p>

<p>&#8195;&#8195; 这个插件提供的功能之一是 <b>修改群系</b>。Amulet 也有，但必须要停服才能编辑，并且在一些服务器上存在着无法显示画面的状况。有了这个插件就能更快、更直观，并且更顺滑地实现群系的变更。</p>

<p>&#8195;&#8195;之二是<b>生成地图</b>。它填补了 Amulet 在地图生成上的一个<b>巨大空缺</b>：只能生成空区块。在mc，区块生成后是不一定被保存到存档的，这就导致一些空区块掺杂到想要操作的区域内，使得大规模结构迁移难以进行。有了这个插件，只要<b>一个指令</b>就能自动生成海量区块并将它们保存进存档，大刀阔斧劈存档不再是梦！</p>

</div>



<h3>&gt; 展示：群系编辑</h3>

<center>

    <img src=https://www.minebbs.com/data/attachments/36/36342-99cdc6a6272ec70ba4e816f3c53b3bea.jpg>

     <p style="text-align:center;font-size:16px"><b>红 雾 异 变</b></p>

     <img src=https://www.minebbs.com/data/attachments/36/36345-77c373e994886e829c84d0c98c36d496.jpg>

     <p style="text-align:center;font-size:16px"><b>血 池 地 狱</b></p>

     <img src=https://www.minebbs.com/attachments/338-png.46703/?hash=5fb935772b887a18d3b9360ebe7a42f8>

     <p style="text-align:center;font-size:16px"><b>西行寺狱狱子</b></p>

     <a href="https://www.minebbs.com/resources/npc-touhou-npc.4616/" target="_blank" title="逛完那边别忘了回来哦owo">NPC行为包下载</a>

</center>



<h3>&gt; 展示：地图生成</h3>



<center>

    <img src=https://www.minebbs.com/attachments/png.46696/ style="width:50%">

     <p style="text-align:center;font-size:16px"><b>自然生成地形会包含许多空区块</b></p>

     <img src=https://www.minebbs.com/attachments/qq-20230616001655-jpg.46694/ style="width:50%">

     <p style="text-align:center;font-size:16px"><b>使用插件完美加载矩形区域内所有区块，强迫症福音</b></p>

</center>



<h3>&gt; 命令一览</h3>



<h4>基本编辑功能</h4>



<div style="font-size:18px">

<ul><li><p><strong>be biome get</strong> [pos] [dimID]</p><ul>

<li>获取某一坐标的群系</li>



</ul></li></ul>

<ul><li><p><strong>be biome set_id</strong> [pos] [dimId] [biomeId]</p><ul>

<li>由id设置某一坐标的群系</li>

</ul></li></ul>



<ul><li><p><strong>be biome set_name</strong> [pos] [dimId] [biomeName]</p><ul>

<li>由名称设置某一坐标的群系</li></ul></li>

</ul><ul>

<li><p><strong>be biomes replace</strong> [startPos] [endPos] [dimId] [originId] [replaceId]</p>

<ul>



<li>将某一长方体区域内，群系id为 [originId] 的群系替换为 [replaceId] 对应的群系。[originId] 为-1时替换所有。</li></ul></li></ul><ul>

<li><p><strong>be biomes replace</strong> [startPos] [endPos] [dimId] [originName] [replaceName]</p><ul>

<li>将某一长方体区域内，群系名称为[originName]的群系替换为[replaceName]对应的群系。[originId] 为“every”时替换所有。</li></ul></li>

</div>



<div style="font-size:18px">

</ul>

<p>&nbsp;</p>



<h4>以区块为最小单元的保存与加载</h4>



<ul>



<li><p><strong>mvc save</strong> [start_x] [start_z] [end_x] [end_z] [dim_id] [file_name] [save]</p>



<ul>



<li>保存区块坐标在[start_x] [start_z] ~ [end_x] [end_z]之间（包含边界）的所有区块的群系。</li>



<li>目录为“plugins/WheatBuilder/Area/file_name”。</li>



<li>还未生成的区块会自动生成，当[save]为true时，这些区块还会被保存到存档。</li>



<li>注意，[dim_id]目前只能为0（主世界）。</li>

</ul></li></ul>



<p></br></p>



<ul>

<li><p><strong>mvc load</strong> [start_x] [start_z] [dim_id] [file_name]</p>

<ul>

<li>将保存的区块群系加载出来，起点是[start_x] [start_z]。</li>

<li>未生成的区块会被自动生成，并保存到存档。</li>

<li>注意，[dim_id]目前只能为0（主世界）。</li>

</ul>

</li>

</ul>

<p></br></p>

<ul>

<li><p><strong>mvc generate</strong> [start_x] [start_z] [end_x] [end_z] [dim_id]</p>

<ul>

<li>加载并将一片区块保存到存档。</li>

</ul>

</li>

</ul>

<p></br></p>

<h3>&gt; 有用的知识</h3>

<ol>

<li><p>对群系的更改需要重新进入世界才能观察到。</p>

</li>

<li><p>dimId：0 主世界；1 地狱；2 末地；</p>

</li>

<li><p>区块和区块坐标</p>

<ul>

<li><p>区块：<a href='https://minecraft.fandom.com/zh/wiki/' target='_blank' class='url'>https://minecraft.fandom.com/zh/wiki/</a>区块</p>

</li>

<li><p>区块坐标：某点所在区块的坐标(cx, cz)等于这个点的坐标(x, z)除以16，向下取整。</p>

</li>

<li><p>如：点(3, 64, 3)处于区块(0, 0)中</p>

<p>&nbsp;</p>

</li>

</ul>

</li>

</ol>

</div>



<h3>&gt; 群系名称和ID</h3>



<p style="font-size:18px">&#8195;&#8195;这里的ID与<a href='https://minecraft.fandom.com/zh/wiki/生物群系/ID'>wiki</a>上描述的有些不同，但name是完全一致的，由于某些特性，只有0~50的id能用在指令中。</p>

</div>

</div>

<div style="font-size:18px">

感谢：<br>

&#8195;&#8195;幻化成疯 提供区块保存方法。

</div>
