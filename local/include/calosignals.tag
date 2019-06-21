<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>AnalysisHelperCore.h</name>
    <path>/export/share/data2/lochp/CaloSignals/local/include/</path>
    <filename>AnalysisHelperCore_8h</filename>
    <includes id="HistUtils_8h" name="HistUtils.h" local="yes" imported="no">HistUtils.h</includes>
    <class kind="class">HistUtils::HistMgr</class>
    <class kind="struct">AnalysisHelper</class>
    <namespace>HistUtils</namespace>
    <member kind="typedef">
      <type>AnalysisHelper</type>
      <name>AH</name>
      <anchorfile>AnalysisHelperCore_8h.html</anchorfile>
      <anchor>a0c2a264adb48da831b33653dcd897b74</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>HistUtils.h</name>
    <path>/export/share/data2/lochp/CaloSignals/local/include/</path>
    <filename>HistUtils_8h</filename>
    <namespace>HistUtils</namespace>
    <member kind="enumeration">
      <type></type>
      <name>Stats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Mean</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110ae57ec394e887bc7762c6476186d7eaf8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Median</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110abf140a74893a9708698f76a4ee416789</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMS</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110a1058d6eb74658ec382948203fe444f2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Unknown</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110ae0759b6d55043d68f3704e920af7c8a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const std::map&lt; int, std::string &gt; &amp;</type>
      <name>fstats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a763ef83bda6c72effba6b89023f6ed51</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const std::string &amp;</type>
      <name>lookupStats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>abdbe4f180f6fd3236fd6f8ceb9166862</anchor>
      <arglist>(Stats s)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>stats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a75d5e50c598254d5415d2b2e1919eae9</anchor>
      <arglist>(TH1D *h, double &amp;m, double &amp;dm, Stats s=Mean)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>stats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a0ae5e8fb9c5a958bacd0827d3d05a7bf</anchor>
      <arglist>(TH1D *h, double &amp;m, double &amp;dl, double &amp;dh, Stats s=Mean)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::vector&lt; TH1D * &gt;</type>
      <name>extract</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a383a62e11470b0d24d33f2c464b72f59</anchor>
      <arglist>(TH2D *d, const std::string &amp;name, int alongAxis)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static TGraphAsymmErrors *</type>
      <name>graph</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a9abc1fea2a65fb19bf19bb1be852d7fa</anchor>
      <arglist>(TH2D *d, const std::string &amp;name, int alongAxis, Stats s=Mean)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scale2D</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>ad08060ea4a76cdf48a806c40b4374954</anchor>
      <arglist>(TH2D *h, double scale)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::string</type>
      <name>printFile</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a58dcc08a0178093c596c86b8b99b5f8d</anchor>
      <arglist>(const std::string &amp;pre, const std::string &amp;post, const std::string &amp;ext=printExt)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>mean</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>af92ee23ad35cba919099610d716bbb93</anchor>
      <arglist>(TGraph *gr, double xmin, double xmax)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::string</type>
      <name>formStr</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a7c2fb7126abd490e2b35d9f0da8e334b</anchor>
      <arglist>(const std::string &amp;dirn, const std::string &amp;pref, const std::string &amp;tag)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::string</type>
      <name>formStr</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a278ca8855cbfe4846e64ea99c1071d87</anchor>
      <arglist>(const std::string &amp;dirn, const std::string &amp;pref, const std::string &amp;tag, const std::string &amp;postf)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>drawCaloBounds</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a5b9e3c8e95d0626bd82c113eaf0ff84d</anchor>
      <arglist>(double ymin, double ymax, int ndc)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::map&lt; int, std::string &gt;</type>
      <name>stats_lu</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aa3bb9be93085f86cfa38e98f525f525b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const std::string</type>
      <name>printDir</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>ab479dbe64897488a53c190bf8eba173c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const std::string</type>
      <name>printExt</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>acb065721325221957da36e6ac87c0062</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>AnalysisHelper</name>
    <filename>structAnalysisHelper.html</filename>
    <member kind="function" static="yes">
      <type>static H *</type>
      <name>book1D</name>
      <anchorfile>structAnalysisHelper.html</anchorfile>
      <anchor>aed9c208e64246584fdd10a188121c137</anchor>
      <arglist>(const std::string &amp;name, const std::string &amp;title, int nxbins, double xmin, double xmax, const std::string xtitle=&quot;&quot;, const std::string &amp;ytitle=&quot;&quot;)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static H *</type>
      <name>book2D</name>
      <anchorfile>structAnalysisHelper.html</anchorfile>
      <anchor>aeaac2042accbd686e35518691e6b181b</anchor>
      <arglist>(const std::string &amp;name, const std::string &amp;title, int nxbins, double xmin, double xmax, int nybins, double ymin, double ymax, const std::string xtitle=&quot;&quot;, const std::string &amp;ytitle=&quot;&quot;, const std::string &amp;ztitle=&quot;&quot;)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>writeHists</name>
      <anchorfile>structAnalysisHelper.html</anchorfile>
      <anchor>a8d8a117fe4692b12e538cf9f7b922e3a</anchor>
      <arglist>(const std::string &amp;fname, const std::string &amp;ftag=&quot;RECREATE&quot;, bool noempty=true)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>HistUtils::HistMgr</name>
    <filename>classHistUtils_1_1HistMgr.html</filename>
    <member kind="function" static="yes">
      <type>static HistMgr *</type>
      <name>instance</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a470a4d54f8b7f7e305aa43fb48f99e9c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a0ddb91771b282ca41a4b7b916f8e7e2c</anchor>
      <arglist>(TH1 *hptr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>remove</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a0f2238621b12aa2e768f804344d15915</anchor>
      <arglist>(TH1 *hptr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>clear</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a43c6a46f880d56f843ecf0b6d920426d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static size_t</type>
      <name>write</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a9af371f98ca15a96b7090e6ab3785998</anchor>
      <arglist>(bool noempty=true)</arglist>
    </member>
    <member kind="function" protection="private">
      <type></type>
      <name>HistMgr</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a1ab05ff224b8260bab8e9f2e4c9c9b12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static HistMgr *</type>
      <name>_instance</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a9941e8757f5e3dabfc24422822857fe6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static std::list&lt; TH1 * &gt;</type>
      <name>_hlist</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a2f35b5aff01c50080e7ea59309f69842</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>add</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a0ddb91771b282ca41a4b7b916f8e7e2c</anchor>
      <arglist>(TH1 *hptr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>remove</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a0f2238621b12aa2e768f804344d15915</anchor>
      <arglist>(TH1 *hptr)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>clear</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a43c6a46f880d56f843ecf0b6d920426d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static size_t</type>
      <name>write</name>
      <anchorfile>classHistUtils_1_1HistMgr.html</anchorfile>
      <anchor>a9af371f98ca15a96b7090e6ab3785998</anchor>
      <arglist>(bool noempty=true)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>HistUtils</name>
    <filename>namespaceHistUtils.html</filename>
    <class kind="class">HistUtils::HistMgr</class>
    <member kind="enumeration">
      <type></type>
      <name>Stats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Mean</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110ae57ec394e887bc7762c6476186d7eaf8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Median</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110abf140a74893a9708698f76a4ee416789</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>RMS</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110a1058d6eb74658ec382948203fe444f2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Unknown</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aee957b08165ec79d9ab22d3985938110ae0759b6d55043d68f3704e920af7c8a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const std::map&lt; int, std::string &gt; &amp;</type>
      <name>fstats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a763ef83bda6c72effba6b89023f6ed51</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const std::string &amp;</type>
      <name>lookupStats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>abdbe4f180f6fd3236fd6f8ceb9166862</anchor>
      <arglist>(Stats s)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>stats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a75d5e50c598254d5415d2b2e1919eae9</anchor>
      <arglist>(TH1D *h, double &amp;m, double &amp;dm, Stats s=Mean)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>stats</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a0ae5e8fb9c5a958bacd0827d3d05a7bf</anchor>
      <arglist>(TH1D *h, double &amp;m, double &amp;dl, double &amp;dh, Stats s=Mean)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::vector&lt; TH1D * &gt;</type>
      <name>extract</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a383a62e11470b0d24d33f2c464b72f59</anchor>
      <arglist>(TH2D *d, const std::string &amp;name, int alongAxis)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static TGraphAsymmErrors *</type>
      <name>graph</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a9abc1fea2a65fb19bf19bb1be852d7fa</anchor>
      <arglist>(TH2D *d, const std::string &amp;name, int alongAxis, Stats s=Mean)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>scale2D</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>ad08060ea4a76cdf48a806c40b4374954</anchor>
      <arglist>(TH2D *h, double scale)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::string</type>
      <name>printFile</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a58dcc08a0178093c596c86b8b99b5f8d</anchor>
      <arglist>(const std::string &amp;pre, const std::string &amp;post, const std::string &amp;ext=printExt)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>mean</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>af92ee23ad35cba919099610d716bbb93</anchor>
      <arglist>(TGraph *gr, double xmin, double xmax)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::string</type>
      <name>formStr</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a7c2fb7126abd490e2b35d9f0da8e334b</anchor>
      <arglist>(const std::string &amp;dirn, const std::string &amp;pref, const std::string &amp;tag)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static std::string</type>
      <name>formStr</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a278ca8855cbfe4846e64ea99c1071d87</anchor>
      <arglist>(const std::string &amp;dirn, const std::string &amp;pref, const std::string &amp;tag, const std::string &amp;postf)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>drawCaloBounds</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>a5b9e3c8e95d0626bd82c113eaf0ff84d</anchor>
      <arglist>(double ymin, double ymax, int ndc)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static std::map&lt; int, std::string &gt;</type>
      <name>stats_lu</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>aa3bb9be93085f86cfa38e98f525f525b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const std::string</type>
      <name>printDir</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>ab479dbe64897488a53c190bf8eba173c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const std::string</type>
      <name>printExt</name>
      <anchorfile>namespaceHistUtils.html</anchorfile>
      <anchor>acb065721325221957da36e6ac87c0062</anchor>
      <arglist></arglist>
    </member>
  </compound>
</tagfile>
