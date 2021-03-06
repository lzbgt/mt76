/*
 * Copyright (C) 2014 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __MT76_MAC_H
#define __MT76_MAC_H

struct mt76_dev;
struct mt76_sta;
struct mt76_vif;
struct mt76_wcid;

struct mt76_txwi;
struct mt76_queue;
struct mt76_txwi_cache;

struct mt76_tx_status {
	u8 valid:1;
	u8 success:1;
	u8 aggr:1;
	u8 ack_req:1;
	u8 wcid;
	u8 pktid;
	u8 retry;
	u16 rate;
} __packed __aligned(2);

struct mt76_tx_info {
	unsigned long jiffies;
	u8 tries;

	u8 wcid;
	u8 pktid;
	u8 retry;
};

struct mt76_rxwi {
	__le32 rxinfo;

	__le32 ctl;

	__le16 tid_sn;
	__le16 rate;

	u8 rssi[4];

	__le32 bbp_rxinfo[4];
};

#define MT_RXINFO_BA			BIT(0)
#define MT_RXINFO_DATA			BIT(1)
#define MT_RXINFO_NULL			BIT(2)
#define MT_RXINFO_FRAG			BIT(3)
#define MT_RXINFO_UNICAST		BIT(4)
#define MT_RXINFO_MULTICAST		BIT(5)
#define MT_RXINFO_BROADCAST		BIT(6)
#define MT_RXINFO_MYBSS			BIT(7)
#define MT_RXINFO_CRCERR		BIT(8)
#define MT_RXINFO_ICVERR		BIT(9)
#define MT_RXINFO_MICERR		BIT(10)
#define MT_RXINFO_AMSDU			BIT(11)
#define MT_RXINFO_HTC			BIT(12)
#define MT_RXINFO_RSSI			BIT(13)
#define MT_RXINFO_L2PAD			BIT(14)
#define MT_RXINFO_AMPDU			BIT(15)
#define MT_RXINFO_DECRYPT		BIT(16)
#define MT_RXINFO_BSSIDX3		BIT(17)
#define MT_RXINFO_WAPI_KEY		BIT(18)
#define MT_RXINFO_PN_LEN		GENMASK(21, 19)
#define MT_RXINFO_SW_FTYPE0		BIT(22)
#define MT_RXINFO_SW_FTYPE1		BIT(23)
#define MT_RXINFO_PROBE_RESP		BIT(24)
#define MT_RXINFO_BEACON		BIT(25)
#define MT_RXINFO_DISASSOC		BIT(26)
#define MT_RXINFO_DEAUTH		BIT(27)
#define MT_RXINFO_ACTION		BIT(28)
#define MT_RXINFO_TCP_SUM_ERR		BIT(30)
#define MT_RXINFO_IP_SUM_ERR		BIT(31)

#define MT_RXWI_CTL_WCID		GENMASK(7, 0)
#define MT_RXWI_CTL_KEY_IDX		GENMASK(9, 8)
#define MT_RXWI_CTL_BSS_IDX		GENMASK(12, 10)
#define MT_RXWI_CTL_UDF			GENMASK(15, 13)
#define MT_RXWI_CTL_MPDU_LEN		GENMASK(29, 16)
#define MT_RXWI_CTL_EOF			BIT(31)

#define MT_RXWI_TID			GENMASK(3, 0)
#define MT_RXWI_SN			GENMASK(15, 4)

#define MT_RXWI_RATE_INDEX		GENMASK(5, 0)
#define MT_RXWI_RATE_LDPC		BIT(6)
#define MT_RXWI_RATE_BW			GENMASK(8, 7)
#define MT_RXWI_RATE_SGI		BIT(9)
#define MT_RXWI_RATE_STBC		BIT(10)
#define MT_RXWI_RATE_LDPC_EXSYM		BIT(11)
#define MT_RXWI_RATE_PHY		GENMASK(15, 13)

#define MT_RATE_INDEX_VHT_IDX		GENMASK(3, 0)
#define MT_RATE_INDEX_VHT_NSS		GENMASK(5, 4)

enum mt76_phy_type {
	MT_PHY_TYPE_CCK,
	MT_PHY_TYPE_OFDM,
	MT_PHY_TYPE_HT,
	MT_PHY_TYPE_HT_GF,
	MT_PHY_TYPE_VHT,
};

enum mt76_phy_bandwith {
	MT_PHY_BW_20,
	MT_PHY_BW_40,
	MT_PHY_BW_80,
};

#define MT_TXWI_FLAGS_FRAG		BIT(0)
#define MT_TXWI_FLAGS_MMPS		BIT(1)
#define MT_TXWI_FLAGS_CFACK		BIT(2)
#define MT_TXWI_FLAGS_TS		BIT(3)
#define MT_TXWI_FLAGS_AMPDU		BIT(4)
#define MT_TXWI_FLAGS_MPDU_DENSITY	GENMASK(7, 5)
#define MT_TXWI_FLAGS_TXOP		GENMASK(9, 8)
#define MT_TXWI_FLAGS_NDPS		BIT(10)
#define MT_TXWI_FLAGS_RTSBWSIG		BIT(11)
#define MT_TXWI_FLAGS_NDP_BW		GENMASK(13, 12)
#define MT_TXWI_FLAGS_SOUND		BIT(14)
#define MT_TXWI_FLAGS_TX_RATE_LUT	BIT(15)

#define MT_TXWI_ACK_CTL_REQ		BIT(0)
#define MT_TXWI_ACK_CTL_NSEQ		BIT(1)
#define MT_TXWI_ACK_CTL_BA_WINDOW	GENMASK(7, 2)

#define MT_TXWI_PKTID_PROBE		BIT(7)

struct mt76_txwi {
	__le16 flags;
	__le16 rate;
	u8 ack_ctl;
	u8 wcid;
	__le16 len_ctl;
	__le32 iv;
	__le32 eiv;
	u8 aid;
	u8 txstream;
	u8 ctl2;
	u8 pktid;
} __packed __aligned(4);


static inline struct mt76_tx_info *
mt76_skb_tx_info(struct sk_buff *skb)
{
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
	return (void *) info->status.status_driver_data;
}

int mt76_mac_reset(struct mt76_dev *dev, bool hard);
int mt76_mac_start(struct mt76_dev *dev);
void mt76_mac_stop(struct mt76_dev *dev, bool force);
void mt76_mac_resume(struct mt76_dev *dev);
void mt76_mac_set_bssid(struct mt76_dev *dev, u8 idx, const u8 *addr);

int mt76_mac_process_rx(struct mt76_dev *dev, struct sk_buff *skb, void *rxwi);
void mt76_mac_write_txwi(struct mt76_dev *dev, struct mt76_txwi *txwi,
			 struct sk_buff *skb, struct mt76_wcid *wcid,
			 struct ieee80211_sta *sta);
void mt76_mac_wcid_setup(struct mt76_dev *dev, u8 idx, u8 vif_idx, u8 *mac);
int mt76_mac_wcid_set_key(struct mt76_dev *dev, u8 idx,
			  struct ieee80211_key_conf *key);
void mt76_mac_wcid_set_rate(struct mt76_dev *dev, struct mt76_wcid *wcid,
			    const struct ieee80211_tx_rate *rate);

int mt76_mac_shared_key_setup(struct mt76_dev *dev, u8 vif_idx, u8 key_idx,
			      struct ieee80211_key_conf *key);

int mt76_insert_hdr_pad(struct sk_buff *skb);
void mt76_remove_hdr_pad(struct sk_buff *skb);
int mt76_mac_skb_tx_overhead(struct mt76_dev *dev, struct sk_buff *skb);

int mt76_mac_set_beacon(struct mt76_dev *dev, u8 vif_idx, struct sk_buff *skb);
void mt76_mac_set_beacon_enable(struct mt76_dev *dev, u8 vif_idx, bool val);

void mt76_mac_queue_txdone(struct mt76_dev *dev, struct sk_buff *skb,
			   struct mt76_txwi *txwi);

void mt76_mac_poll_tx_status(struct mt76_dev *dev, bool irq);
void mt76_mac_process_tx_status_fifo(struct mt76_dev *dev);

void mt76_mac_work(struct work_struct *work);

#endif
